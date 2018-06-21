package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

public class MCSourceDisplay extends SVGImage {

	public MCSourceDisplay(BeltParameterLayout parent) {
		super(parent, "MC_A_Label.svg", "MC_B_Label.svg", "MC_C_Label.svg", "MC_D_Label.svg");
	}

	@Override
	public BeltParameterLayout getParent() {
		return (BeltParameterLayout) super.getParent();
	}

	@Override
	public int getSelectedPhase() {
		/*-
		Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();

		 if (p instanceof ModulatableParameter) {
		 ModulatableParameter m = (ModulatableParameter) p;
		 MacroControls s = m.getModulationSource();

		 switch (s) {
		 case A:
		 return 0;
		 case B:
		 return 1;
		 case C:
		 return 2;
		 case D:
		 return 3;

		 default:
		 break;

		 }
		 }-*/

		return -1;
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		if (getParent().isOneOf(BeltParameterLayout.Mode.mcValue, BeltParameterLayout.Mode.mcAmount, BeltParameterLayout.Mode.mcSource,
				BeltParameterLayout.Mode.paramValue))
			getParent().setMode(BeltParameterLayout.Mode.modulateableParameter);
		else if (getSelectedPhase() != -1)
			getParent().setMode(BeltParameterLayout.Mode.paramValue);

		return this;
	}
}
