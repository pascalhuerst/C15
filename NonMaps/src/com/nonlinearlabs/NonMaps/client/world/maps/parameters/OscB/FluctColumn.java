package com.nonlinearlabs.NonMaps.client.world.maps.parameters.OscB;

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

class FluctColumn extends ParameterColumn {

	private class Fluctuation extends ModulateableKnob {

		private Fluctuation(MapsLayout parent) {
			super(parent);
		}

		@Override
		public int getParameterID() {
			return 87;
		}
	}

	private class EnvelopeC extends ModulationSourceHighPriority {

		private EnvelopeC(MapsLayout parent) {
			super(parent);
			addChild(new LabelModulationSource(this));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 89;
		}
	}

	private class Phase extends Parameter {

		private Phase(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this));
			addChild(new KnobSmall(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 302;
		}
	}

	FluctColumn(MapsLayout parent) {
		super(parent);
		addChild(new Fluctuation(this));
		addChild(new EnvelopeC(this));
		addChild(new Phase(this));
	}
}
