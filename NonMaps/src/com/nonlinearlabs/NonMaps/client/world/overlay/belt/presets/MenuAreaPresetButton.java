package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

public class MenuAreaPresetButton extends SVGImage {

	public MenuAreaPresetButton(MenuArea parent) {
		super(parent, "Menu_Preset_Enabled_S.svg", "Menu_Preset_Active_S.svg", "Menu_Preset_Disabled_S.svg", "Menu_A_Enabled.svg",
				"Menu_A_Active.svg", "Menu_A_Disabled.svg");
	}

	@Override
	public Control mouseDown(Position pos) {
		Preset p = getPreset();
		if (p != null) {
			Overlay o = getOverlay();
			return o.setContextMenu(pos, new PresetContextMenu(o, p));
		}
		return super.click(pos);
	}

	private Overlay getOverlay() {
		return NonMaps.get().getNonLinearWorld().getViewport().getOverlay();
	}

	@Override
	public Control onContextMenu(Position pos) {
		Preset p = getPreset();
		if (p != null) {
			Overlay o = getOverlay();
			return o.setContextMenu(pos, new PresetContextMenu(o, p));
		}
		return super.click(pos);
	}

	Preset getPreset() {
		PresetManager pm = getPresetManager();
		String bankUUID = pm.getSelectedBank();

		if (bankUUID != null) {
			Bank bank = pm.findBank(bankUUID);
			if (bank != null) {
				String presetUUID = bank.getPresetList().getSelectedPreset();
				return bank.getPresetList().findPreset(presetUUID);
			}
		}
		return null;
	}

	private PresetManager getPresetManager() {
		return NonMaps.get().getNonLinearWorld().getPresetManager();
	}

	boolean hasPreset() {
		PresetManager pm = getPresetManager();
		String bankUUID = pm.getSelectedBank();

		if (bankUUID != null) {
			Bank bank = pm.findBank(bankUUID);
			if (bank != null) {
				String presetUUID = bank.getPresetList().getSelectedPreset();
				return bank.getPresetList().findPreset(presetUUID) != null;
			}
		}
		return false;
	}

	@Override
	public int getSelectedPhase() {
		int ret = 0;

		if (hasPreset())
			ret = drawStates.normal.ordinal();
		else
			ret = drawStates.disabled.ordinal();

		if (((MenuArea) getParent()).isSmall() == false)
			ret += 3;
		return ret;
	}
}
