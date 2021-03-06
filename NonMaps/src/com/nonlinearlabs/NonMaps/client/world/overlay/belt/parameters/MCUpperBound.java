package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter.Initiator;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.MacroControlParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.value.QuantizedClippedValue;

public class MCUpperBound extends MCSomething {

	QuantizedClippedValue dummyValue;

	public MCUpperBound(Control parent) {
		super(parent);
		addChild(middle = new MCUpperBoundLabel(this));
	}

	@Override
	public MCUpperBoundLabel getMiddle() {
		return (MCUpperBoundLabel) super.getMiddle();
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();

		if (p instanceof ModulatableParameter) {
			ModulatableParameter m = (ModulatableParameter) p;
			createDummyValue(m);

			getParent().setMode(BeltParameterLayout.Mode.mcAmount);

			if (m.getModulationSource() != MacroControls.NONE)
				getParent().setValueChanger(dummyValue.startUserEdit(getPixRect().getWidth()));
		}

		return getParent();
	}

	private void createDummyValue(ModulatableParameter p) {
		dummyValue = new QuantizedClippedValue(new QuantizedClippedValue.ChangeListener() {

			@Override
			public void onQuantizedValueChanged(Initiator initiator, double oldQuantizedValue, double newRight) {
				Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();

				if (p instanceof ModulatableParameter) {
					ModulatableParameter modulatedParam = (ModulatableParameter) p;
					MacroControls s = modulatedParam.getModulationSource();
					if (s != MacroControls.NONE) {
						MacroControlParameter mc = getNonMaps().getNonLinearWorld().getParameterEditor().getMacroControls().getControl(s);

						double modAmount = modulatedParam.getModulationAmount().getClippedValue();

						if (modulatedParam.isBiPolar())
							modAmount *= 2;

						double srcValue = mc.getValue().getClippedValue();
						double value = modulatedParam.getValue().getClippedValue();
						double modLeft = value - modAmount * srcValue;

						double newModAmount = newRight - modLeft;
						double newValue = modLeft + newModAmount * srcValue;
						newValue = modulatedParam.getValue().clip(newValue);
						newValue = modulatedParam.getValue().getQuantizedValue(newValue, true);

						if (modulatedParam.isBiPolar())
							newModAmount /= 2;

						modulatedParam.getModulationAmount().setRawValue(Initiator.INDIRECT_USER_ACTION, newModAmount);
						modulatedParam.getValue().setRawValue(Initiator.INDIRECT_USER_ACTION, newValue);
						NonMaps.theMaps.getServerProxy().setModulationAmountAndValue(modulatedParam, newModAmount, newValue);
					}
				}
			}

			@Override
			public void onClippedValueChanged(Initiator initiator, double oldClippedValue, double newClippedValue) {
			}

			@Override
			public void onRawValueChanged(Initiator initiator, double oldRawValue, double newRawValue) {
			}
		});

		dummyValue.setCoarseDenominator(p.getValue().getCoarseDenominator());
		dummyValue.setFineDenominator(p.getValue().getFineDenominator());
		dummyValue.setBipolar(p.getValue().isBipolar());
		dummyValue.setRawValue(Initiator.INDIRECT_USER_ACTION, getMiddle().calcBound());
	}

	@Override
	public Control doubleClick() {
		Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();

		if (p instanceof ModulatableParameter) {
			ModulatableParameter m = (ModulatableParameter) p;
			createDummyValue(m);
			dummyValue.setToDefault(Parameter.Initiator.EXPLICIT_USER_ACTION);
		}

		return this;
	}

	@Override
	protected void inc(ModulatableParameter m, boolean fine) {
		createDummyValue(m);
		dummyValue.inc(Initiator.EXPLICIT_USER_ACTION, fine);
	}

	@Override
	protected void dec(ModulatableParameter m, boolean fine) {
		createDummyValue(m);
		dummyValue.dec(Initiator.EXPLICIT_USER_ACTION, fine);
	}
}
