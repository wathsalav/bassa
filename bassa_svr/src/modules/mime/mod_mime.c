#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <confuse.h>

#include <mod_mime.h>
#include <bassa_module_manager.h>

char *magicfile = NULL;

bassa_module_region module_region = COR_REGION;
bassa_exec_point module_execpoint = COR_X;


__attribute__((destructor))void fini_mod_mime()
{
  if (magicfile)
    {
      free (magicfile);
      magicfile = NULL;
    }
}

void
module_init (char *conf)
{
  if (!conf)
    return;
  char *magicf = NULL;
  cfg_opt_t opts[] = {
    CFG_SIMPLE_STR("magic_file", &magicf)
  };
  cfg_t *cfg = cfg_init (opts, 0);
  cfg_parse (cfg, conf);
  cfg_free (cfg);
  magicfile = strdup(magicf);
  if (magicf)
    {
      free (magicf);
      magicf = NULL;
    }
}

char*
mod_mime_gettype (char *filename)
{
  magic_t mt = magic_open (MAGIC_MIME);
  magic_load (mt, magicfile);
  const char *mime_type = magic_file (mt, filename);
  char *mimet = NULL;
  if (mime_type)
    mimet = strdup (mime_type); 
  magic_close (mt);
  return mimet;
}
