package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationRoutingParameter;

public class AmountsCol1 extends AmountsCol {

	public AmountsCol1(MapsLayout parent) {
		super(parent);
		addChild(new ModulationRoutingParameter(this, 255, 254, 243));
		addChild(new ModulationRoutingParameter(this, 260, 259, 243));
		addChild(new ModulationRoutingParameter(this, 265, 264, 243));
		addChild(new ModulationRoutingParameter(this, 270, 269, 243));
		addChild(new ModulationRoutingParameter(this, 275, 274, 243));
		addChild(new ModulationRoutingParameter(this, 280, 279, 243));
		addChild(new RibbonModulationRoutingParameter(this, 285, 284, 243));
		addChild(new RibbonModulationRoutingParameter(this, 290, 289, 243));
	}
}
