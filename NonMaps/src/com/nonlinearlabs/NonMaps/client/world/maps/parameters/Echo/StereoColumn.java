package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Echo;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class StereoColumn extends ParameterColumn {

	private class Stereo extends ModulateableKnob {

		private Stereo(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 227;
		}
	}

	private class HiCut extends Parameter {

		private HiCut(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 232;
		}
	}

	StereoColumn(MapsLayout parent) {
		super(parent);
		addChild(new Stereo(this));
		addChild(new HiCut(this));
	}
}
