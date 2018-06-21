package com.nonlinearlabs.NonMaps.client.world.maps.parameters.CombFilter;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulateableNumericalControl;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.NumericalControlSmall;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SmallParameterName;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;

class CombFilterCol2 extends ParameterColumn {

	private class Pitch extends ModulatableParameter {

		private Pitch(MapsLayout parent) {
			super(parent);
			addChild(new ModulateableNumericalControl(this));
		}

		@Override
		public int getParameterID() {
			return 115;
		}
	}

	private class EnvC extends ModulationSourceHighPriority {

		private EnvC(MapsLayout parent) {
			super(parent);
			addChild(new LabelModulationSource(this));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return 118;
		}
	}

	private class Keytracking extends ModulationSourceHighPriority {

		private Keytracking(MapsLayout parent) {
			super(parent);
			addChild(new SmallParameterName(this));
			addChild(new NumericalControlSmall(this));
		}

		@Override
		public int getParameterID() {
			return 117;
		}
	}

	CombFilterCol2(MapsLayout parent) {
		super(parent);
		addChild(new Pitch(this));
		addChild(new EnvC(this));
		addChild(new Keytracking(this));

	}
}
