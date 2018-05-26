#include <map>
#include "ArpPattern.h"
#include "ArpIntegrityException.h"

const Identifier ArpPattern::TREEID_PATTERN = Identifier("pattern"); // NOLINT
const Identifier ArpPattern::TREEID_TIMEBASE = Identifier("timebase"); // NOLINT
const Identifier ArpPattern::TREEID_LOOP_LENGTH = Identifier("loopLength"); // NOLINT
const Identifier ArpPattern::TREEID_NOTES = Identifier("notes"); // NOLINT

ArpPattern::ArpPattern(int timebase) {
    this->timebase = timebase;
    this->loopLength = 0;
    this->notes = std::vector<ArpNote>();
}

ArpPattern::~ArpPattern() = default;


int ArpPattern::getTimebase() {
    return this->timebase;
}

std::vector<ArpNote> &ArpPattern::getNotes() {
    return this->notes;
}

std::vector<ArpEvent> ArpPattern::build() {
    std::map<int64, ArpEvent> eventMap;

    for (ArpNote &note : this->notes) {
        if (note.endPoint <= note.startPoint) {
            throw ArpIntegrityException("A note cannot end before it starts and has to be at least one pulse long!");
        }

        ArpEvent &onEvent = eventMap[note.startPoint];
        onEvent.time = note.startPoint;
        onEvent.ons.push_back(&(note.data));

        ArpEvent &offEvent = eventMap[note.endPoint];
        offEvent.time = note.endPoint;
        offEvent.offs.push_back(&(note.data));
    }

    std::vector<ArpEvent> result;
    for (std::pair<int, ArpEvent> pair : eventMap) {
        result.push_back(pair.second);
    }

    return result;
}

ValueTree ArpPattern::toValueTree() {
    ValueTree result = ValueTree(TREEID_PATTERN);

    result.setProperty(TREEID_TIMEBASE, this->timebase, nullptr);
    result.setProperty(TREEID_LOOP_LENGTH, this->loopLength, nullptr);

    ValueTree noteTree = result.getOrCreateChildWithName(TREEID_NOTES, nullptr);
    for (ArpNote note : this->notes) {
        noteTree.appendChild(note.toValueTree(), nullptr);
    }

    return result;
}


ArpPattern ArpPattern::fromValueTree(ValueTree &tree) {
    if (!tree.isValid() || !tree.hasType(TREEID_PATTERN)) {
        throw std::invalid_argument("Input tree must be valid and of the correct type!");
    }

    int timebase = DEFAULT_TIMEBASE;
    if (tree.hasProperty(TREEID_TIMEBASE)) {
        timebase = tree.getProperty(TREEID_TIMEBASE);
    }

    ArpPattern result = ArpPattern(timebase);

    if (tree.hasProperty(TREEID_LOOP_LENGTH)) {
        result.loopLength = tree.getProperty(TREEID_LOOP_LENGTH);
    }

    ValueTree notesTree = tree.getChildWithName(TREEID_NOTES);
    if (notesTree.isValid()) {
        for (int i = 0; i < notesTree.getNumChildren(); i++) {
            ValueTree noteTree = notesTree.getChild(i);
            result.notes.push_back(ArpNote::fromValueTree(noteTree));
        }
    }

    return result;
}
