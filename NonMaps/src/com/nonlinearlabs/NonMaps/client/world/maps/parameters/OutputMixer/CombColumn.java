package com.nonlinearlabs.NonMaps.client.world.maps.parameters.OutputMixer;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class CombColumn extends ParameterColumn {

	private class Comb extends ModulatableSlider {

		private Comb(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 175;
		}
	}

	private class Pan extends Parameter {

		private Pan(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 177;
		}
	}

	CombColumn(MapsLayout parent) {
		super(parent);
		addChild(new Comb(this));
		addChild(new Pan(this));
	}
}
