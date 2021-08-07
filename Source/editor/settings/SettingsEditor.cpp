//
// This file is part of LibreArp
//
// LibreArp is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// LibreArp is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see https://librearp.gitlab.io/license/.
//

#include "SettingsEditor.h"

SettingsEditor::SettingsEditor(LibreArp& p) : processor(p) {
    updateCheckToggle.setButtonText("Check for updates automatically");
    updateCheckToggle.setTooltip("When this is enabled, the plugin will automatically check for updates when loaded, at most once a day.");
    updateCheckToggle.onStateChange = [this] {
        processor.getGlobals().setCheckForUpdatesEnabled(updateCheckToggle.getToggleState());
    };
    addAndMakeVisible(updateCheckToggle);
}

void SettingsEditor::resized() {
    updateLayout();
}

void SettingsEditor::updateSettingsValues() {
    updateCheckToggle.setToggleState(processor.getGlobals().isCheckForUpdatesEnabled(), juce::NotificationType::dontSendNotification);
}

void SettingsEditor::visibilityChanged() {
    Component::visibilityChanged();
    updateLayout();
}

void SettingsEditor::updateLayout() {
    if (!isVisible()) {
        return;
    }

    updateSettingsValues();

    auto area = getLocalBounds().reduced(8);
    updateCheckToggle.setBounds(area.removeFromTop(24));
}
