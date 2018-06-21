package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.Setup;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.VelocityCurve;
import com.nonlinearlabs.NonMaps.client.useCases.SystemSettings;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;

public class VelocityCurveSetting extends Setting {

	int choice = 0;

	protected VelocityCurveSetting(DeviceSettingsPage parent) {
		super(parent, "Velocity Curve");
	}

	@Override
	public void init() {
		super.init();

		Setup.get().systemSettings.velocityCurve.onChange(new Function<Setup.VelocityCurve, Boolean>() {

			@Override
			public Boolean apply(Setup.VelocityCurve t) {
				choice = t.ordinal();
				return true;
			}
		});
	}

	@Override
	protected OverlayControl createSettingsControl() {
		return new SettingsMenu(this) {

			@Override
			protected String[] getChoices() {
				String ret[] = { "Very Soft", "Soft", "Normal", "Hard", "Very Hard" };
				return ret;
			}

			@Override
			protected int getChoice() {
				return choice;
			}

			@Override
			protected void chose(int c, boolean fire) {
				choice = c;
				invalidate(INVALIDATION_FLAG_UI_CHANGED);

				if (fire)
					SystemSettings.get().setVelocityCurve(VelocityCurve.values()[choice]);
			}
		};
	}
}
