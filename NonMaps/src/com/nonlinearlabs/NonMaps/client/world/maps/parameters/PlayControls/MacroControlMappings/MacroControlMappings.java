package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControlMappings;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModuleHeader;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupVertical;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PhysicalControlParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.HardwareSources.Pedal;

public class MacroControlMappings extends ParameterGroupVertical {

	private MacroControlMappingControls controls;

	public MacroControlMappings(MapsLayout parent) {
		super(parent);
		addChild(new LabelModuleHeader(this, "Hardware Sources and Amounts"));
		addChild(controls = new MacroControlMappingControls(this));
	}

	@Override
	public String getID() {
		return "MCM";
	}

	public void onReturningModeChanged(PhysicalControlParameter src) {
		controls.onReturningModeChanged(src);
	}

	public Pedal getPedal(int i) {
		return controls.getPedal(i);
	}
}
