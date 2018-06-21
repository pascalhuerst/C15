package com.nonlinearlabs.NonMaps.client.world.maps.parameters.EnvA;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class EnvACol6 extends ParameterColumn {

	private class Release extends ModulateableKnob {

		private Release(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 10;
		}
	}

	private class Velocity extends ModulationSourceHighPriority {

		private Velocity(MapsLayout parent) {
			super(parent);
			addChild(new LabelModulationSource(this));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 16;
		}
	}

	EnvACol6(MapsLayout parent) {
		super(parent);
		addChild(new Release(this));
		addChild(new Velocity(this));
	}
}
