/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 * 2022 Beaver (GEGL starfield)
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_string (string, _("Graph2"), TUTORIALG)
    ui_meta     ("role", "output-extent")

#define TUTORIALG \
" id=1 src  aux=[ ref=1 color ] crop "\




property_color (value, _("Color"), "#ffffff")
    description (_("The color to paint over the input"))
    ui_meta     ("role", "#ffffff")
    ui_meta     ("role", "output-extent")

property_int  (holdness, _("Dulling"), 8)
  value_range (8, 8)
  description (_("A high value lowers the randomness of the noise"))
    ui_meta     ("role", "output-extent")

property_double (saturation_distance, _("Add background stars and enhance color"), 0.035)
  value_range   (0.025, 0.046)


property_double (value_distance, _("Amount of Stars"), 0.06)
  value_range   (0.043, 0.065)

property_seed   (seed, _("Random seed"), rand)


property_double (in_high, _("High input"), 0.33)
    description (_("Input luminance level to become white"))
    ui_range    (0.33, 0.33)
    ui_meta     ("role", "output-extent")

property_double (out_high, _("Make Stars Brighter"), 4.15)
    description (_("Highest luminance level in output"))
    ui_range    (3.95, 4.2)

property_double (gamma, _("Size range of Stars"), 14)
   description(_("Slide up to only show large stars. Slide down to only show small stars"))
   ui_range (0, 40)


property_double (std_dev, _("Blur (alt use for making snow) "), 1.0)
   description (_("Standard deviation (spatial scale factor)"))
   value_range (0.0, 7.0)
   ui_range    (0.24, 7.0)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")

property_double (saturation, _("Add Color to Stars"), 0.0)
    description(_("Scale, strength of effect"))
    value_range (0.0, 1.0)
    ui_range (0.0, 1.0)

property_double (factor, _("Zoom Motion Blur"), 0.00)
    value_range (0, 0.30)
    ui_range    (0.0, 0.30)
    ui_gamma    (2.0)


#else

#define GEGL_OP_META
#define GEGL_OP_NAME     starfield
#define GEGL_OP_C_SOURCE starfield.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *graph, *color, *noise, *invert, *levels, *gamma, *blur, *glow, *saturation, *zmb;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  color = gegl_node_new_child (gegl,
                                  "operation", "gegl:color-overlay",
                                  NULL);

  graph = gegl_node_new_child (gegl,
                                  "operation", "gegl:gegl",
                                  NULL);


  noise = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-hsv",
                                  NULL);

  invert = gegl_node_new_child (gegl,
                                  "operation", "gegl:invert",
                                  NULL);

  levels = gegl_node_new_child (gegl,
                                  "operation", "gegl:levels",
                                  NULL);

  gamma = gegl_node_new_child (gegl,
                                  "operation", "gegl:gamma",
                                  NULL);

  blur = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);

  saturation = gegl_node_new_child (gegl,
                                  "operation", "gegl:saturation",
                                  NULL);

  glow = gegl_node_new_child (gegl,
                                  "operation", "gegl:softglow",
                                  NULL);

  zmb = gegl_node_new_child (gegl,
                                  "operation", "gegl:motion-blur-zoom",
                                  NULL);







    gegl_operation_meta_redirect (operation, "value", color, "value");
    gegl_operation_meta_redirect (operation, "saturation_distance", noise, "saturation-distance");
    gegl_operation_meta_redirect (operation, "value_distance", noise, "value-distance");
    gegl_operation_meta_redirect (operation, "holdness", noise, "holdness");
    gegl_operation_meta_redirect (operation, "seed", noise, "seed");
    gegl_operation_meta_redirect (operation, "gamma", gamma, "value");
    gegl_operation_meta_redirect (operation, "std_dev", blur, "std-dev-x");
    gegl_operation_meta_redirect (operation, "std_dev", blur, "std-dev-y");
    gegl_operation_meta_redirect (operation, "out_high", levels, "out-high");
    gegl_operation_meta_redirect (operation, "in_high", levels, "in-high");
    gegl_operation_meta_redirect (operation, "saturation", saturation, "scale");
    gegl_operation_meta_redirect (operation, "factor", zmb, "factor");
    gegl_operation_meta_redirect (operation, "string",  graph, "string");





  gegl_node_link_many (input, graph, color, noise, invert, levels, gamma, blur, glow, saturation, zmb, output, NULL);




}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:starfield",
    "title",       _("Starfield"),
    "categories",  "Aristic",
    "reference-hash", "45eak6vgah28vf20fno25sb2ac",
    "description", _("Render a Starry Night with GEGL. "
                     ""),
    NULL);
}

#endif
