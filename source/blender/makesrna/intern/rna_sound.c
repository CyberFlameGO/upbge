/* SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup RNA
 */

#include <stdlib.h>

#include "RNA_define.h"

#include "rna_internal.h"

#include "DNA_property_types.h"
#include "DNA_sound_types.h"

#ifdef RNA_RUNTIME

#  include "BKE_context.h"
#  include "BKE_sound.h"

#  include "DEG_depsgraph.h"

#  include "SEQ_sequencer.h"

static void rna_Sound_update(Main *UNUSED(bmain), Scene *UNUSED(scene), PointerRNA *ptr)
{
  bSound *sound = (bSound *)ptr->data;
  DEG_id_tag_update(&sound->id, ID_RECALC_AUDIO);
}

static void rna_Sound_caching_update(Main *bmain, Scene *scene, PointerRNA *ptr)
{
  rna_Sound_update(bmain, scene, ptr);
  DEG_id_tag_update(&scene->id, ID_RECALC_SEQUENCER_STRIPS);
}

#else

static void rna_def_sound(BlenderRNA *brna)
{
  StructRNA *srna;
  PropertyRNA *prop;

  srna = RNA_def_struct(brna, "Sound", "ID");
  RNA_def_struct_sdna(srna, "bSound");
  RNA_def_struct_ui_text(
      srna, "Sound", "Sound data-block referencing an external or packed sound file");
  RNA_def_struct_ui_icon(srna, ICON_SOUND);

  // rna_def_ipo_common(srna);

  prop = RNA_def_property(srna, "filepath", PROP_STRING, PROP_FILEPATH);
  RNA_def_property_string_sdna(prop, NULL, "filepath");
  RNA_def_property_ui_text(prop, "File Path", "Sound sample file used by this Sound data-block");
  RNA_def_property_update(prop, 0, "rna_Sound_update");

  prop = RNA_def_property(srna, "packed_file", PROP_POINTER, PROP_NONE);
  RNA_def_property_pointer_sdna(prop, NULL, "packedfile");
  RNA_def_property_ui_text(prop, "Packed File", "");

  prop = RNA_def_property(srna, "use_memory_cache", PROP_BOOLEAN, PROP_NONE);
  RNA_def_property_boolean_sdna(prop, NULL, "flags", SOUND_FLAGS_CACHING);
  RNA_def_property_ui_text(prop, "Caching", "The sound file is decoded and loaded into RAM");
  RNA_def_property_update(prop, 0, "rna_Sound_caching_update");

  prop = RNA_def_property(srna, "use_mono", PROP_BOOLEAN, PROP_NONE);
  RNA_def_property_boolean_sdna(prop, NULL, "flags", SOUND_FLAGS_MONO);
  RNA_def_property_ui_text(
      prop,
      "Mono",
      "If the file contains multiple audio channels they are rendered to a single one");
  RNA_def_property_update(prop, 0, "rna_Sound_update");

  RNA_api_sound(srna);
}

void RNA_def_sound(BlenderRNA *brna)
{
  rna_def_sound(brna);
}

#endif
