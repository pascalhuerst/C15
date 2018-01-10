package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.nonlinearlabs.NonMaps.client.world.Control;

public class SoftwareUpdateAvailable extends Setting {

	public SoftwareUpdateAvailable(Control parent) {
		super(parent, "Software Update available");
	}

	@Override
	protected SettingsControl createSettingsControl() {
		return new SoftwareUpdateAvailableSettings(this);
	}

}
