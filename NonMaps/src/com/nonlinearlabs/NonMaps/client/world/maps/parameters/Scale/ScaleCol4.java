package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Scale;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;

class ScaleCol4 extends ScaleCol {

	ScaleCol4(MapsLayout parent) {
		super(parent);
		addChild(new ScaleParameter(this) {

			@Override
			public int getParameterID() {
				return 315;
			}
		});

		addChild(new ScaleParameter(this) {

			@Override
			public int getParameterID() {
				return 319;
			}
		});

		addChild(new ScaleParameter(this) {

			@Override
			public int getParameterID() {
				return 323;
			}
		});
	}
}
