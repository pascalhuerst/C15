package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;

public class PresetBeltLayouter {
	BeltPresetLayout layout;

	public PresetBeltLayouter(BeltPresetLayout layout) {
		this.layout = layout;
	}

	public boolean doLayout(double w, double h) {
		return false;
	}

	protected void layoutLockSymbol(double w) {
		double lockSymbolSizeInPixel = Millimeter.toPixels(layout.lock.getLockSizeInMM() + 1);
		layout.lock.doLayout(w - lockSymbolSizeInPixel, 0, lockSymbolSizeInPixel, lockSymbolSizeInPixel);
	}

	protected void hide(OverlayControl ctrl) {
		ctrl.doLayout(0, 0, 0, 0);
	}

	public double getMinBankControlWidth() {
		return Millimeter.toPixels(60);
	}

	public double getMaxBankControlWidth() {
		return Millimeter.toPixels(100);
	}
}
