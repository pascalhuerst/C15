package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsControl;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationRoutingParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PhysicalControlParameter;

public class AmountsCol extends ParameterColumn {

	public AmountsCol(MapsLayout parent) {
		super(parent);
	}

	public void onReturningModeChanged(PhysicalControlParameter src) {
		for (MapsControl c : getChildren()) {
			if (c instanceof ModulationRoutingParameter) {
				ModulationRoutingParameter p = (ModulationRoutingParameter) c;
				p.onReturningModeChanged(src);
			}
		}
	}

}
