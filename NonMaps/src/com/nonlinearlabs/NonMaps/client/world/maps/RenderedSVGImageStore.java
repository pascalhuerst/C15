package com.nonlinearlabs.NonMaps.client.world.maps;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;

import com.google.gwt.core.client.Scheduler;
import com.google.gwt.core.client.Scheduler.RepeatingCommand;
import com.google.gwt.dom.client.ImageElement;
import com.google.gwt.user.client.ui.Image;
import com.google.gwt.xml.client.Document;
import com.google.gwt.xml.client.Element;
import com.google.gwt.xml.client.Node;
import com.google.gwt.xml.client.NodeList;
import com.nonlinearlabs.NonMaps.client.world.maps.SVGImageStore.SVGDownloadHandler;

public class RenderedSVGImageStore {
	static private RenderedSVGImageStore self = new RenderedSVGImageStore();

	static public RenderedSVGImageStore get() {
		return self;
	}

	private class PendingRenderJob implements Runnable {
		private final int width;
		private final int height;
		private final String name;
		private final ArrayList<SVGRenderHandler> handler = new ArrayList<SVGRenderHandler>();

		private PendingRenderJob(int width, int height, String name, SVGRenderHandler handler) {
			this.width = width;
			this.height = height;
			this.name = name;
			this.handler.add(handler);
		}

		@Override
		public void run() {
			SVGImageStore.get().loadImage(name, new SVGDownloadHandler() {

				@Override
				public void onSVGDownloaded(Document doc) {
					final Image img = new Image();
					final ImageElement face = ImageElement.as(img.getElement());
					NodeList svgs = doc.getElementsByTagName("svg");
					if (svgs.getLength() > 0) {
						Node svg = svgs.item(0);
						Element svgElement = (Element) svg;

						svgElement.setAttribute("width", width + "px");
						svgElement.setAttribute("height", height + "px");
						String moddedSvg = "data:image/svg+xml;base64," + base64Encode(svg.toString());
						face.setSrc(moddedSvg);

						if (img.getWidth() != 0 && img.getHeight() != 0) {
							String key = name + width + height;
							cache.put(key, face);

							for (SVGRenderHandler h : handler)
								h.onSVGRendered(face);
						}
					}
				}
			});
		}
	}

	public interface SVGRenderHandler {
		public void onSVGRendered(ImageElement img);
	}

	private HashMap<String, ImageElement> cache = new HashMap<String, ImageElement>();
	private LinkedList<PendingRenderJob> pendingJobs = new LinkedList<PendingRenderJob>();

	public RenderedSVGImageStore() {
	}

	private static native String base64Encode(String v) /*-{
														return btoa(v);
														}-*/;

	private static native String base64Decode(String v) /*-{
														return atob(v);
														}-*/;

	public ImageElement render(String name, int width, int height, SVGRenderHandler handler) {
		for (Iterator<PendingRenderJob> iter = pendingJobs.listIterator(); iter.hasNext();) {
			PendingRenderJob a = iter.next();

			if (a.handler.contains(handler)) {
				if (a.width == width && a.height == height && a.name.equals(name)) {
					return null;
				} else {
					iter.remove();
				}
			}
		}

		String key = name + width + height;
		ImageElement img = cache.get(key);

		if (img == null)
			downloadAndCache(name, width, height, handler);
		else if (img.getWidth() != width || img.getHeight() != height)
			downloadAndCache(name, width, height, handler);

		return img;
	}

	public void cancel(SVGRenderHandler handler) {
		for (Iterator<PendingRenderJob> iter = pendingJobs.listIterator(); iter.hasNext();) {
			PendingRenderJob a = iter.next();
			if (a.handler == handler) {
				iter.remove();
			}
		}
	}

	private void downloadAndCache(final String name, final int width, final int height, final SVGRenderHandler handler) {
		for (Iterator<PendingRenderJob> iter = pendingJobs.listIterator(); iter.hasNext();) {
			PendingRenderJob a = iter.next();

			if (a.width == width && a.height == height && a.name.equals(name)) {
				a.handler.add(handler);
				return;
			}
		}

		PendingRenderJob r = new PendingRenderJob(width, height, name, handler);

		boolean schedule = pendingJobs.isEmpty();
		pendingJobs.add(r);

		if (schedule)
			scheduleJobs();
	}

	private void scheduleJobs() {
		Scheduler.get().scheduleIncremental(new RepeatingCommand() {

			@Override
			public boolean execute() {
				Runnable r = pendingJobs.pollFirst();

				if (r != null) {
					r.run();
					pendingJobs.remove(r);
					return true;
				}

				return false;
			}
		});
	}
}
