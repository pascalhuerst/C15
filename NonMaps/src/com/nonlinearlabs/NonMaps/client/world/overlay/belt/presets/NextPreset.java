package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter.Initiator;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage.drawStates;

class NextPreset extends SVGImage {

	NextPreset(PrevNextPresetButtons parent) {
		super(parent, "Preset_Down_Enabled.svg", "Preset_Down_Active.svg", "Preset_Down_Disabled.svg");
	}

	@Override
	public Control click(Position eventPoint) {
		doPresetSelection();
		return this;
	}

	private void doPresetSelection() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		pm.selectNextPreset(Initiator.EXPLICIT_USER_ACTION);
	}

	@Override
	public Control longLeftPress(Position eventPoint) {
		ButtonRepeat.get().start(new Runnable() {

			@Override
			public void run() {
				doPresetSelection();
			}
		});
		return super.longLeftPress(eventPoint);
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		return this;
	}

	@Override
	public void onMouseLost() {
		ButtonRepeat.get().cancel();
		super.onMouseLost();
	}

	@Override
	public int getSelectedPhase() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();

		Bank b = pm.findBank(pm.getSelectedBank());
		if(b != null)
			if(b.getPresetList().getPresetCount() == 0)
				return drawStates.disabled.ordinal();
		
		if (!pm.canNext())
			return drawStates.disabled.ordinal();

		return super.getSelectedPhase();
	}
}
