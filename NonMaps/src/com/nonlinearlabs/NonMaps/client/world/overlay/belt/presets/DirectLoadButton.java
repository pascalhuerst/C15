package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.ServerProxy;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

class DirectLoadButton extends SVGImage {

	private boolean married;

	DirectLoadButton(OverlayLayout parent) {
		super(parent, "Link_Enabled.svg", "Link_Active.svg", "Link_Disabled.svg");
	}

	public boolean isInStoreSelectMode() {
		return NonMaps.get().getNonLinearWorld().getPresetManager().isInStoreSelectMode();
	}

	@Override
	public int getSelectedPhase() {
		if (isInStoreSelectMode())
			return drawStates.disabled.ordinal();
		else if (married)
			return drawStates.active.ordinal();

		return drawStates.normal.ordinal();
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		if (isInStoreSelectMode())
			return this;

		married = !married;
		NonMaps.theMaps.getServerProxy().setSetting("AutoLoadSelectedPreset", married ? "on" : "off");
		return this;
	}

	public void update(Node settingsNode) {
		String str = ServerProxy.getChildText(settingsNode, "AutoLoadSelectedPreset", "value");
		if (str != null && !str.isEmpty()) {
			boolean m = str.toLowerCase().equals("on");
			if (married != m) {
				married = m;
				invalidate(INVALIDATION_FLAG_UI_CHANGED);
				requestLayout();
			}
		}
	}
}