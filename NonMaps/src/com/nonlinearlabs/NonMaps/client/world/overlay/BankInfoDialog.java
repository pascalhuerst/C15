package com.nonlinearlabs.NonMaps.client.world.overlay;

import java.util.Date;

import com.google.gwt.event.dom.client.BlurEvent;
import com.google.gwt.event.dom.client.BlurHandler;
import com.google.gwt.event.dom.client.FocusEvent;
import com.google.gwt.event.dom.client.FocusHandler;
import com.google.gwt.event.dom.client.KeyCodes;
import com.google.gwt.event.dom.client.KeyPressEvent;
import com.google.gwt.event.dom.client.KeyPressHandler;
import com.google.gwt.i18n.client.DateTimeFormat;
import com.google.gwt.i18n.client.DateTimeFormat.PredefinedFormat;
import com.google.gwt.user.client.ui.FlexTable;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.TextArea;
import com.google.gwt.user.client.ui.TextBox;
import com.google.gwt.user.client.ui.Widget;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;

public class BankInfoDialog extends GWTDialog {

	private static BankInfoDialog theDialog;

	private TextBox name;
	private TextArea comment;
	private Label size;
	private Label lastChange;
	private Label importFileDate;
	private Label importFileName;
	Bank theBank;

	private Widget haveFocus = null;

	private BankInfoDialog() {
		RootPanel.get().add(this);

		getElement().addClassName("bank-info-dialog");

		initalShow();

		setAnimationEnabled(true);
		setGlassEnabled(false);
		setModal(false);

		addHeader("Bank Info");
		addContent();

		initialSetup();
	}

	private void initialSetup() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		updateInfo(pm.findBank(pm.getSelectedBank()));
	}

	private void addRow(FlexTable panel, String name, Widget content) {
		int c = panel.getRowCount();
		panel.setWidget(c, 0, new Label(name));
		panel.setWidget(c, 1, content);
	}

	private void addContent() {
		FlexTable panel = new FlexTable();
		addRow(panel, "Name", name = new TextBox());
		addRow(panel, "Comment", comment = new TextArea());
		addRow(panel, "Size", size = new Label(""));
		addRow(panel, "Date of Last Change", lastChange = new Label(""));
		addRow(panel, "Date of Import File", importFileDate = new Label(""));
		addRow(panel, "Name of Import File", importFileName = new Label(""));

		comment.addFocusHandler(new FocusHandler() {

			@Override
			public void onFocus(FocusEvent event) {
				setFocus(comment);
			}
		});

		comment.addBlurHandler(new BlurHandler() {

			@Override
			public void onBlur(BlurEvent event) {
				haveFocus = null;

				if (theBank != null) {
					String oldInfo = theBank.getAttribute("Comment");

					if (!oldInfo.equals(comment.getText())) {
						NonMaps.theMaps.getServerProxy().setBankAttribute(theBank, "Comment", comment.getText());
					}
				}
			}
		});

		name.addFocusHandler(new FocusHandler() {

			@Override
			public void onFocus(FocusEvent event) {
				setFocus(name);
			}
		});

		name.addBlurHandler(new BlurHandler() {

			@Override
			public void onBlur(BlurEvent event) {
				haveFocus = null;

				if (theBank != null) {
					String oldName = theBank.getCurrentName();

					if (!oldName.equals(name.getText())) {
						NonMaps.theMaps.getServerProxy().renameBank(theBank.getUUID(), name.getText());
					}
				}
			}
		});

		name.addKeyPressHandler(new KeyPressHandler() {

			@Override
			public void onKeyPress(KeyPressEvent arg0) {
				if (arg0.getCharCode() == KeyCodes.KEY_ENTER) {
					name.setFocus(false);
					comment.setFocus(true);
				}
			}
		});

		setWidget(panel);
	}

	@Override
	protected void commit() {
		hide();
		theDialog = null;
		NonMaps.theMaps.captureFocus();
		NonMaps.theMaps.getNonLinearWorld().requestLayout();
	}

	public static void toggle() {
		if (theDialog != null) {
			theDialog.commit();
		} else {
			if (!NonMaps.theMaps.getNonLinearWorld().getPresetManager().isEmpty())
				theDialog = new BankInfoDialog();
		}
	}

	public static boolean isShown() {
		return theDialog != null;
	}

	public static void update(Bank bank) {
		if (theDialog != null)
			theDialog.updateInfo(bank);
	}

	private void updateInfo(Bank bank) {
		if (bank != null) {
			String bankName = bank.getCurrentName();
			String commentText = bank.getAttribute("Comment");

			if (haveFocus != comment) {
				if (!commentText.equals(comment.getText())) {
					comment.setText(commentText);
				}
			}

			if (haveFocus != name) {
				if (!bankName.equals(name.getText())) {
					name.setText(bankName);
				}
			}

			size.setText(Integer.toString(bank.getPresetCount()));

			try {
				lastChange.setText(localizeTime(bank.getDateOfLastChange()));
			} catch (Exception e) {
				lastChange.setText("---");
			}

			try {
				importFileDate.setText(localizeTime(bank.getAttribute("Date of Import File")));
			} catch (Exception e) {
				importFileDate.setText("---");
			}

			importFileName.setText(bank.getAttribute("Name of Import File"));

			centerIfOutOfView();
		}
	}

	private String localizeTime(String iso) {
		try {
			DateTimeFormat f = DateTimeFormat.getFormat("yyyy-MM-ddTHH:mm:ssZZZZ");
			Date d = f.parse(iso);
			DateTimeFormat locale = DateTimeFormat.getFormat(PredefinedFormat.DATE_TIME_SHORT);
			return locale.format(d);
		} catch (Exception e) {
			return iso;
		}
	}

	static int lastPopupLeft = -1;
	static int lastPopupTop = -1;

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

	private void setFocus(Widget w) {
		haveFocus = w;
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		theBank = pm.findBank(pm.getSelectedBank());
	}

	public static void update() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		update(pm.findBank(pm.getSelectedBank()));
	}
}