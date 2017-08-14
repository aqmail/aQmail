#include <shadow.h>

int main()
{
  struct spwd *spw;

  spw = getspnam("");
  puts(spw->sp_pwdp);
}
