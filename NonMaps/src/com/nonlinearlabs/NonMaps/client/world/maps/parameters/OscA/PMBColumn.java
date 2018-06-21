package com.nonlinearlabs.NonMaps.client.world.maps.parameters.OscA;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.KnobSmall;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class PMBColumn extends ParameterColumn {

	private class PMB extends ModulateableKnob {

		private PMB(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 64;
		}
	}

	private class EnvB extends ModulationSourceHighPriority {

		private EnvB(MapsLayout parent) {
			super(parent);
			addChild(new LabelModulationSource(this));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 66;
		}
	}

	private class ShaperMix extends Parameter {

		private ShaperMix(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 67;
		}
	}

	PMBColumn(MapsLayout parent) {
		super(parent);
		addChild(new PMB(this));
		addChild(new EnvB(this));
		addChild(new ShaperMix(this));
	}
}
