package com.nonlinearlabs.NonMaps.client.world.overlay.html.presetSearch;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.overlay.GWTDialog;

public class PresetSearchDialog extends GWTDialog {

	static int lastPopupLeft = -1;
	static int lastPopupTop = -1;
	static PresetSearchDialog theSearch;

	private PresetSearchDialog() {
		super();
		com.nonlinearlabs.NonMaps.client.useCases.PresetSearch.get().startSearch();
		add(new PresetSearch());
		setAnimationEnabled(true);
		setGlassEnabled(false);
		setModal(false);
	}

	public static void showDialog() {
		if (theSearch == null) {
			theSearch = new PresetSearchDialog();
			theSearch.setModal(false);
			theSearch.addHeader("Search Presets for ...");
			theSearch.initalShow();
		}
	}

	public static void hideDialog() {
		if (theSearch != null) {
			theSearch.commit();
		}
	}

	public static void toggle() {
		if (theSearch != null)
			hideDialog();
		else
			showDialog();
	}

	public static boolean isShown() {
		return theSearch != null;
	}

	@Override
	protected void setLastPopupPos(int popupLeft, int popupTop) {
		lastPopupLeft = popupLeft;
		lastPopupTop = popupTop;
	}

	@Override
	protected int getLastPopupPosTop() {
		return lastPopupTop;
	}

	@Override
	protected int getLastPopupPosLeft() {
		return lastPopupLeft;
	}

	@Override
	protected void commit() {
		hide();
		com.nonlinearlabs.NonMaps.client.useCases.PresetSearch.get().finishSearch();
		theSearch = null;
		NonMaps.theMaps.captureFocus();
		NonMaps.theMaps.getNonLinearWorld().requestLayout();
	}
}
