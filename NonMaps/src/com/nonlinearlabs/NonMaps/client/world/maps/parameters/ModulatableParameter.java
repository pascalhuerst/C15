package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Checksum;
import com.nonlinearlabs.NonMaps.client.ColorTable;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.MacroControlParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;

public abstract class ModulatableParameter extends Parameter {
	/*-
	 private ModulationAmount amount = new ModulationAmount(new ModulationAmount.ChangeListener() {

	 @Override
	 public void onClippedValueChanged(Initiator initiator, double oldClippedValue, double newClippedValue) {
	 }

	 @Override
	 public void onQuantizedValueChanged(Initiator initiator, double oldQuantizedValue, double newQuantizedValue) {
	 if (initiator == Initiator.EXPLICIT_USER_ACTION)
	 getNonMaps().getServerProxy().setModulationAmount(ModulatableParameter.this);

	 notifyListeners();
	 invalidate(INVALIDATION_FLAG_UI_CHANGED);
	 }

	 @Override
	 public void onRawValueChanged(Initiator initiator, double oldRawValue, double newRawValue) {
	 }
	 });-*/

	public ModulatableParameter(MapsLayout parent) {
		super(parent);
		addChild(new ParameterName(this));
	}

	public ModulatableParameter(MapsLayout parent, String name) {
		super(parent);
		addChild(new ParameterName(this, name));
	}

	@Override
	public void getStateHash(Checksum crc) {
		super.getStateHash(crc);
		crc.eat(getPresenter().hash);
		crc.eat(isSelectedParameterMyMacroControl());
	}

	public boolean isSelectedParameterMyMacroControl() {
		/*-Parameter selectedParam = getNonMaps().getNonLinearWorld().getParameterEditor().getSelection();

		if (selectedParam != null && selectedParam instanceof MacroControlParameter) {
			MacroControlParameter myParam = getNonMaps().getNonLinearWorld().getParameterEditor().getMacroControls()
					.getControl(getModulationSource());

			if (myParam != null && myParam.equals(selectedParam))
				return true;
		}
		-*/
		return false;
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);

		if (isSelectedParameterMyMacroControl())
			getPixRect().drawRoundedRect(ctx, getBackgroundRoundings(), toXPixels(5), toXPixels(1.5), null,
					ColorTable.getMacroControlTargetColor());
	}

	public void setModulationSource(MacroControls src, Initiator initiator) {
		/*-
		if (modulationSource != src) {

			MacroControlParameter modSrcParam = getModulationSourceParameter();

			if (modSrcParam != null)
				modSrcParam.removeModulatableParameter(this);

			modulationSource = src;

			modSrcParam = getModulationSourceParameter();

			if (modSrcParam != null)
				modSrcParam.addModulatableParameter(this);

			if (initiator == Initiator.EXPLICIT_USER_ACTION)
				getNonMaps().getServerProxy().setModulationSource(this);

			getValue().setRawValue(Initiator.INDIRECT_USER_ACTION, getValue().getQuantizedClipped());
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}-*/
	}

	private MacroControlParameter getModulationSourceParameter() {
		// return
		// getNonMaps().getNonLinearWorld().getParameterEditor().getMacroControls().getControl(getModulationSource());
		return null;
	}

	protected String getModSourceString() {
		/*-
		switch (getModulationSource()) {
		case A:
			return "\u24b6";

		case B:
			return "\u24b7";

		case C:
			return "\u24b8";

		case D:
			return "\u24b9";

		default:
		}-*/

		return "";
	}

	public void applyModulation(Initiator initiator, double diff) {
		// getValue().applyModulation(initiator, amount.getQuantizedClipped() *
		// diff);
	}

	public void modulationAmountInc(boolean fine) {
		// amount.inc(Initiator.EXPLICIT_USER_ACTION, fine);
	}

	public void modulationAmountDec(boolean fine) {
		// amount.dec(Initiator.EXPLICIT_USER_ACTION, fine);
	}

}
