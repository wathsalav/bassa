#include <bassa_sec.h>

int bassa_sec_setuid(char *user)
{
  if (!user)
    return -1;
  struct passwd* pw = getpwnam(user);
  if (!pw)
    return -1;
  int ret = setuid(pw->pw_uid);
  return ret;
}

int bassa_sec_setgid(char *cgroup)
{
  if (!cgroup)
    return -1;
  struct group *grp = getgrnam(cgroup);
  if (!grp)
    return -1;
  int ret = setgid(grp->gr_gid);
  return ret;
}

int bassa_sec_setowner(char *user, char *cgroup, char *file)
{
  if (!user || !cgroup || !file)
    return -1;
  struct passwd* pw = getpwnam(user);
  if (!pw)
    return -1;
  struct group *grp = getgrnam(cgroup);
  if (!grp)
    return -1;
  int ret = chown(file, pw->pw_uid, grp->gr_gid);
  return ret;
}

int bassa_sec_setmode(bassa_sec_mode *mode, char *file)
{
}
