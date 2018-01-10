package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.PresetInfoDialog;

public class MenuAreaInfoButton extends MenuAreaButton {

	public MenuAreaInfoButton(MenuArea parent) {
		super(parent, "Menu_Info_Disabled_S.svg", "Menu_Info_Enabled_S.svg", "Menu_Info_Active_S.svg", "Menu_Info_Disabled_L.svg",
				"Menu_Info_Enabled_L.svg", "Menu_Info_Active_L.svg");
	}

	@Override
	public MenuArea getParent() {
		return (MenuArea) super.getParent();
	}

	@Override
	public Control click(Position eventPoint) {
		PresetInfoDialog.toggle();
		return this;
	}

	@Override
	protected State getState() {
		return PresetInfoDialog.isShown() ? State.Active : State.Enabled;
	}
}
