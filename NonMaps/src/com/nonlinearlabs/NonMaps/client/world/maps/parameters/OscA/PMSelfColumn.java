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

class PMSelfColumn extends ParameterColumn {

	private class PMSelf extends ModulateableKnob {

		private PMSelf(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 60;
		}
	}

	private class EnvA extends ModulationSourceHighPriority {

		private EnvA(MapsLayout parent) {
			super(parent);
			addChild(new LabelModulationSource(this));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 62;
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
			return 63;
		}
	}

	PMSelfColumn(MapsLayout parent) {
		super(parent);
		addChild(new PMSelf(this));
		addChild(new EnvA(this));
		addChild(new ShaperMix(this));
	}
}
