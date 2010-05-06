/*
 *
 * Copyright (C) 2010 Intel Corp
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

/* This is largely based on the moblin SysTray application.  Many thanks! */

#include <config.h>
#include <ctype.h>
#include <stdlib.h>
#include <signal.h>
#include <glib/gi18n-lib.h>
#include <glib/gstdio.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <mx/mx-gtk.h>

#include "abrt-cc-panel.h"

#define ABRT_PATH "/etc/abrt/reporting-settings.conf"

static int is_reporting_enabled(void)
{
  GKeyFile *f;
  GKeyFileFlags flags;
  GError *error = NULL;
  gboolean allow_report = FALSE;

  if (g_file_test(ABRT_PATH, G_FILE_TEST_EXISTS) < 0)
  {
      g_warning("Unable to open: %s", ABRT_PATH);
      return -1;
  }

  f = g_key_file_new();
  flags = G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS;

  if (!g_key_file_load_from_file(f, ABRT_PATH, flags, &error))
  {
    g_warning("%s", error->message);
    return -1;
  }

  allow_report = g_key_file_get_boolean(f, "Global", "Allow-Report", &error);
  if (error != NULL)
  {
    g_warning("%s", error->message);
    return -1;
  }

  return allow_report;
}

struct _AbrtCcPanelPrivate
{
  GtkWidget    *frame;
};

#define GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ABRT_TYPE_CC_PANEL, AbrtCcPanelPrivate))

G_DEFINE_DYNAMIC_TYPE (AbrtCcPanel, abrt_cc_panel, CC_TYPE_PANEL);

static void
abrt_cc_panel_switch_flipped_cb (MxGtkLightSwitch *sw,
                                 gboolean          on,
                                 AbrtCcPanel      *panel)
{
  GKeyFile *f;
  GKeyFileFlags flags;
  GError *error = NULL;
  gchar *d;
  FILE *file;
  gchar *path = g_build_filename (ABRT_PATH, NULL);

  if (g_file_test(ABRT_PATH, G_FILE_TEST_EXISTS) < 0)
  {
      g_warning("Unable to open: %s", ABRT_PATH);
      return;
  }

  f = g_key_file_new ();
  flags = G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS;

  if (!g_key_file_load_from_file (f, ABRT_PATH, flags, &error))
  {
      g_warning("%s", error->message);
      return;
  }

  if (on)
      g_key_file_set_boolean(f, "Global", "Allow-Report", TRUE);
  else
      g_key_file_set_boolean(f, "Global", "Allow-Report", FALSE);

  d = g_key_file_to_data (f, NULL, NULL); 

  file = g_fopen (path, "wb"); 
  if (file == NULL) {
      g_warning("Unable to open: %s", path);
      goto out;
  }

  g_fprintf (file, "%s", d);
  fclose (file); 

out:
  g_free (d); 
  g_key_file_free (f); 
}

static void
abrt_cc_panel_make_contents (AbrtCcPanel *panel)
{
  AbrtCcPanelPrivate *priv = panel->priv;
  GtkWidget          *vbox, *hbox, *label, *button, *frame;
  gchar              *txt;

  frame = mx_gtk_frame_new ();
  gtk_widget_show (frame);

  vbox = gtk_vbox_new (FALSE, 15);
  gtk_widget_show (vbox);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);
  gtk_container_add (GTK_CONTAINER (frame), vbox);

  txt = g_strdup_printf ("<span font_desc=\"Liberation Sans Bold 18px\""
                           "foreground=\"#3e3e3e\">%s</span>",
                           _ ("Crash report configuration"));

  label = gtk_label_new (NULL);
  gtk_label_set_markup (GTK_LABEL (label), txt);
  gtk_widget_show (label);
  gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);

  g_free (txt);

  hbox = gtk_hbox_new (FALSE, 10);
  gtk_widget_show (hbox);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

  txt = g_strdup_printf ("<span font_desc=\"Liberation Sans Bold\""
                           "foreground=\"#3e3e3e\">%s</span>",
                           _ ("Automatically submit application crash reports to MeeGo reporting database?"));

  label = gtk_label_new (NULL);
  gtk_label_set_markup (GTK_LABEL (label), txt);

  gtk_widget_show (label);
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);

  g_free (txt);

  button = mx_gtk_light_switch_new ();
  gtk_widget_show (button);
  gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);

  if (is_reporting_enabled () == 1)
    mx_gtk_light_switch_set_active (MX_GTK_LIGHT_SWITCH (button), TRUE);
  else
    mx_gtk_light_switch_set_active (MX_GTK_LIGHT_SWITCH (button), FALSE);

  g_signal_connect (button, "switch-flipped",
                    G_CALLBACK (abrt_cc_panel_switch_flipped_cb),
                    panel);

  label = gtk_label_new (_("PRIVACY NOTE: Enabling this option will cause your system to "
                           "submit certain application output to the corewatcher.org website."
                           "This will allow the information to be viewed by developers as well "
                           "as regular users.  The information submittted includes a backtrace, "
                           "which is an application's crash signature. However, due to the nature "
                           "of backtraces, it may happen that a few instances of user input may be "
                           "sent together with the backtrace."));

  gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  gtk_widget_show (label);
  gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);

  priv->frame = frame;
}

static void
abrt_cc_panel_active_changed (CcPanel  *base_panel,
                              gboolean is_active)
{
  /* AbrtCcPanel *panel = ABRT_CC_PANEL (base_panel); */
  static gboolean populated = FALSE;

  if (is_active && !populated)
    {
      populated = TRUE;
    }
}

static void
abrt_cc_panel_init (AbrtCcPanel *self)
{
  self->priv = GET_PRIVATE (self);

  abrt_cc_panel_make_contents (self);

  gtk_widget_show (self->priv->frame);
  gtk_container_add (GTK_CONTAINER (self), self->priv->frame);
}

static GObject *
abrt_cc_panel_constructor (GType                  type,
                           guint                  n_construct_properties,
                           GObjectConstructParam *construct_properties)
{
  AbrtCcPanel *panel;

  panel = ABRT_CC_PANEL (G_OBJECT_CLASS (abrt_cc_panel_parent_class)->constructor
                          (type, n_construct_properties, construct_properties));

  g_object_set (panel,
                "display-name", _("Crash Reporting Configuration"),
                "id", "abrt-properties.desktop",
                NULL);

  return G_OBJECT (panel);
}

static void
abrt_cc_panel_class_init (AbrtCcPanelClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  CcPanelClass *panel_class = CC_PANEL_CLASS (klass);

  object_class->constructor = abrt_cc_panel_constructor;

  panel_class->active_changed = abrt_cc_panel_active_changed;

  g_type_class_add_private (klass, sizeof (AbrtCcPanelPrivate));
}

static void
abrt_cc_panel_class_finalize (AbrtCcPanelClass *klass)
{
}

void
abrt_cc_panel_register (GIOModule *module)
{
  abrt_cc_panel_register_type (G_TYPE_MODULE (module));
  g_io_extension_point_implement (CC_PANEL_EXTENSION_POINT_NAME,
                                  ABRT_TYPE_CC_PANEL,
                                  "desktopsettings",
                                  10);
}

