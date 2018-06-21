package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import java.util.HashMap;

import com.google.gwt.event.dom.client.KeyDownEvent;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.NonLinearWorld;
import com.nonlinearlabs.NonMaps.client.world.maps.LayoutResizingHorizontal;
import com.nonlinearlabs.NonMaps.client.world.maps.LayoutResizingVertical;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter.Initiator;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Cabinet.Cabinet;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.CombFilter.CombFilter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Echo.Echo;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.EnvA.EnvA;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.EnvB.EnvB;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.EnvC.EnvC;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.FBMixer.FBMixer;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Flanger.Flanger;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.GapFilter.GapFilter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Master.Master;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.OscA.OscA;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.OscB.OscB;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.OutputMixer.OutputMixer;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.PlayControls;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.Macros;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Reverb.Reverb;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SVFilter.SVFilter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Scale.Scale;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ShapeA.ShapeA;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ShapeB.ShapeB;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Unison.Unison;

public class ParameterEditor extends LayoutResizingVertical {

	private PlayControls playControls;
	private static ParameterEditor theEditor = null;

	private class PlayControlsArea extends ResizingHorizontalCenteringLayout {

		private PlayControlsArea(ParameterEditor parent) {
			super(parent);
			playControls = addChild(new PlayControls(this));
		}
	}

	private class SynthParameters extends ResizingHorizontalCenteringLayout {

		private SynthParameters(ParameterEditor parent) {
			super(parent);
			addChild(new ParamEditorCol1());
			addChild(new SpacerLarge(this));
			addChild(new ParamEditorCol2());
			addChild(new SpacerLarge(this));
			addChild(new ParamEditorCol3());
			addChild(new SpacerLarge(this));
			addChild(new ParamEditorCol4());
		}

		private class ParamEditorCol1 extends LayoutResizingVertical {

			public ParamEditorCol1() {
				super(SynthParameters.this);
				addChild(new EnvA(this));
				addChild(new SpacerLarge(this));
				addChild(new EnvC(this));
				addChild(new SpacerLarge(this));
				addChild(new EnvB(this));
			}

			@Override
			public Direction getLayoutDirection() {
				return Direction.RIGHT;
			}

		}

		private class ParamEditorCol2 extends LayoutResizingVertical {

			public ParamEditorCol2() {
				super(SynthParameters.this);
				addChild(new OscA(this));
				addChild(new SpacerLarge(this));
				addChild(new ShapeA(this));
				addChild(new SpacerLarge(this));
				addChild(new ShapeB(this));
				addChild(new SpacerLarge(this));
				addChild(new OscB(this));
			}
		}

		private class ParamEditorCol3 extends LayoutResizingVertical {

			public ParamEditorCol3() {
				super(SynthParameters.this);
				addChild(new FBMixer(this));
				addChild(new SpacerLarge(this));
				addChild(new CombFilter(this));
				addChild(new SpacerLarge(this));
				addChild(new SVFilter(this));
				addChild(new SpacerLarge(this));
				addChild(new OutputMixer(this));
			}
		}

		private class ParamEditorCol4 extends LayoutResizingVertical {

			private class CabinetAndGapFilter extends LayoutResizingHorizontal {

				public CabinetAndGapFilter() {
					super(SynthParameters.this);

					addChild(new Cabinet(this));
					addChild(new SpacerLarge(this));
					addChild(new GapFilter(this));
				}
			}

			private class EchoAndReverb extends LayoutResizingHorizontal {

				public EchoAndReverb() {
					super(SynthParameters.this);

					addChild(new Echo(this));
					addChild(new SpacerLarge(this));
					addChild(new Reverb(this));
				}
			}

			private class UnisonAndMaster extends LayoutResizingHorizontal {

				public UnisonAndMaster() {
					super(SynthParameters.this);

					addChild(new Unison(this));
					addChild(new SpacerLarge(this));
					addChild(new Master(this));
					addChild(new SpacerLarge(this));
					addChild(new Scale(this));
				}
			}

			private ParamEditorCol4() {
				super(SynthParameters.this);

				addChild(new Flanger(this));
				addChild(new SpacerLarge(this));
				addChild(new CabinetAndGapFilter());

				addChild(new SpacerLarge(this));
				addChild(new EchoAndReverb());
				addChild(new SpacerLarge(this) {
					@Override
					protected double getBasicWidth() {
						return super.getBasicWidth() * 8;
					}
				});

				addChild(new UnisonAndMaster());
			}

			@Override
			public Direction getLayoutDirection() {
				return Direction.LEFT;
			}
		}
	}

	public ParameterEditor(NonLinearWorld parent) {
		super(parent);

		assert (theEditor == null);
		theEditor = this;

		addChild(new SpacerLarge(this) {
			@Override
			protected double getBasicHeight() {
				return 10;
			}
		});

		addChild(new PlayControlsArea(this));
		addChild(new SpacerLarge(this));
		addChild(new SpacerLarge(this));
		addChild(new SynthParameters(this));
	}

	public Control onKey(final KeyDownEvent event) {

		int keyCode = event.getNativeEvent().getKeyCode();

		if (keyCode == com.google.gwt.event.dom.client.KeyCodes.KEY_E && getNonMaps().getNonLinearWorld().isCtrlDown()) {
			toggleVisibility();
			return this;
		}
		return null;
	}

	public static ParameterEditor get() {
		return theEditor;
	}

	public void select(Initiator initiator, Parameter sel) {
		/*-
		if (selectedObject != sel) {
			Parameter oldSel = selectedObject;
			selectedObject = sel;

			for (SelectionListener listener : listeners) {
				listener.onSelectionChanged(oldSel, sel);
			}

			/*-
			if (initiator == Initiator.EXPLICIT_USER_ACTION)
				getNonMaps().getServerProxy().onParameterSelectionChanged(this);


			if (NonMaps.theMaps.getNonLinearWorld().getSettings().isOneOf("SelectionAutoScroll", "on", "parameter", "parameter-and-preset"))
				scrollToSelectedParameter();

			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}-*/
	}

	private void scrollToSelectedParameter() {
		/*-
		if (!selectedObject.isVisible())
			((MapsLayout) selectedObject.getParameterGroup()).scrollToMakeFullyVisible();
			-*/
	}

	public void updateHash(Node node) {
		/*-String hash = node.getAttributes().getNamedItem("hash").getNodeValue();
		if (!hash.equals(this.hash)) {
			this.hash = hash;
		}-*/
	}

	private HashMap<Integer, Parameter> parameterMap = new HashMap<Integer, Parameter>();

	public void registerSelectable(Parameter sel) {
		Parameter pl = (Parameter) sel;
		parameterMap.put(pl.getParameterID(), pl);
	}

	Parameter findSelectable(int parameterID) {
		return parameterMap.get(parameterID);
	}

	public Macros getMacroControls() {
		return playControls.getMacroControls();
	}

	public Parameter findParameter(int parameterID) {
		return (Parameter) findSelectable(parameterID);
	}

	public PlayControls getPlayControls() {
		return playControls;
	}

	public boolean isEditorVisible() {
		return isVisible();
	}

	public void toggleVisibility() {
		setVisible(!isIntendedVisible());
	}

	public boolean isAnyParameterLocked() {
		Control c = recurseChildren(new ControlFinder() {
			@Override
			public boolean onWayDownFound(Control ctrl) {
				if (ctrl instanceof Parameter) {
					Parameter p = (Parameter) ctrl;
					if (p.getPresenter().locked) {
						return true;
					}
				}
				return false;
			}
		});

		return c != null;
	}

	public boolean areAllParametersLocked() {
		for (Parameter p : parameterMap.values()) {
			if (p.getPresenter().locked == false)
				return false;
		}
		return true;
	}
}
