/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
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

#ifndef __ABRT_CC_PANEL_H__
#define __ABRT_CC_PANEL_H__

#include <gtk/gtk.h>
#include <libgnome-control-center-extension/cc-panel.h>

G_BEGIN_DECLS

#define ABRT_TYPE_CC_PANEL         (abrt_cc_panel_get_type ())
#define ABRT_CC_PANEL(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), ABRT_TYPE_CC_PANEL, AbrtCcPanel))
#define ABRT_CC_PANEL_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), ABRT_TYPE_CC_PANEL, AbrtCcPanelClass))
#define ABRT_IS_CC_PANEL(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), ABRT_TYPE_CC_PANEL))
#define ABRT_IS_CC_PANEL_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), ABRT_TYPE_CC_PANEL))
#define ABRT_CC_PANEL_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), ABRT_TYPE_CC_PANEL, AbrtCcPanelClass))

typedef struct _AbrtCcPanelPrivate AbrtCcPanelPrivate;

typedef struct
{
        CcPanel              parent;
        AbrtCcPanelPrivate *priv;
} AbrtCcPanel;

typedef struct
{
        CcPanelClass parent_class;
} AbrtCcPanelClass;

GType abrt_cc_panel_get_type (void);

void abrt_cc_panel_register (GIOModule *module);

G_END_DECLS

#endif /* __ABRT_CC_PANEL_H__ */

