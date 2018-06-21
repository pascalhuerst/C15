package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationRoutingParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterEditor;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PhysicalControlParameter;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.RibbonContextMenu;
import com.nonlinearlabs.NonMaps.client.world.overlay.setup.ContextMenusSetting;

public abstract class Ribbon extends PhysicalControlParameter {

	private int currentMode = 0;

	Ribbon(HardwareSourcesCol1 parent) {
		super(parent);
	}

	@Override
	public RibbonContextMenu createContextMenu(Overlay o) {
		ContextMenusSetting contextMenuSettings = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay().getSetup()
				.getContextMenuSettings();
		if (contextMenuSettings.isEnabled()) {
			return new RibbonContextMenu(o, this);
		}
		return null;
	}

	@Override
	public boolean hasContextMenu() {
		return true;
	}

	@Override
	public ReturnMode getReturnMode() {
		switch (currentMode) {
		case 0:
			return ReturnMode.None;

		case 1:
			return ReturnMode.Center;
		}
		return ReturnMode.None;
	}

	private void onRibbonModeChanged() {
		ParameterEditor eddi = (ParameterEditor) getSelectionRoot();
		/*-
		SourcesAndAmounts mappings = (SourcesAndAmounts) eddi.findParameterGroup("MCM");
		mappings.onReturningModeChanged(this);
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
		-*/
	}

	@Override
	public void addModulationRoutingParameter(final ModulationRoutingParameter modulationRoutingParameter) {
		super.addModulationRoutingParameter(modulationRoutingParameter);

		/*-
		int target = modulationRoutingParameter.getTargetParameterID();
		Parameter mc = NonMaps.theMaps.getNonLinearWorld().getParameterEditor().findParameter(target);
		mc.addListener(new ParameterListener() {

			@Override
			public void onParameterChanged(QuantizedClippedValue newValue) {
				if (getReturnMode() == ReturnMode.None && modulationRoutingParameter.getValue().getQuantizedClipped() != 0.0) {
					getValue().setRawValue(Initiator.INDIRECT_USER_ACTION, newValue.getQuantizedClipped());
				}
			}
		});-*/
	}

}