package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterEditor;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PhysicalControlParameter;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.PedalContextMenu;

public abstract class Pedal extends PhysicalControlParameter {

	private int currentMode = 0;

	Pedal(MapsLayout parent) {
		super(parent);
	}

	@Override
	public PedalContextMenu createContextMenu(Overlay o) {
		return new PedalContextMenu(o, this);
	}

	@Override
	public boolean hasContextMenu() {
		return true;
	}

	private void onPedalModeChanged() {
		ParameterEditor eddi = (ParameterEditor) getSelectionRoot();
		/*-
		SourcesAndAmounts mappings = (SourcesAndAmounts) eddi.findParameterGroup("MCM");
		mappings.onReturningModeChanged(this);
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
		-*/
	}

	@Override
	public ReturnMode getReturnMode() {
		switch (currentMode) {
		case 0:
			return ReturnMode.None;

		case 1:
			return ReturnMode.Zero;

		case 2:
			return ReturnMode.Center;
		}
		return ReturnMode.None;
	}

}