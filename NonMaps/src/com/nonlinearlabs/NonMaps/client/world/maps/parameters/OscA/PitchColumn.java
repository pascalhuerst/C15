package com.nonlinearlabs.NonMaps.client.world.maps.parameters.OscA;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableNumericalControl;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.NumericalControlSmall;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class PitchColumn extends ParameterColumn {

	private class Pitch extends ModulatableParameter {

		private Pitch(MapsLayout parent) {
			super(parent);
			addChild(new ModulateableNumericalControl(this));
		}

		@Override
		public int getParameterID() {
			return 53;
		}

		@Override
		public double getPadding() {
			return 2;
		}
	}

	private class Keytracking extends Parameter {

		private Keytracking(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this));
			addChild(new NumericalControlSmall(this));
		}

		@Override
		public int getParameterID() {
			return 55;
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
			return 56;
		}
	}

	PitchColumn(MapsLayout parent) {
		super(parent);
		addChild(new Pitch(this));
		addChild(new EnvelopeC(this));
		addChild(new Keytracking(this));
	}
}
