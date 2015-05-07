#!/bin/bash

valid="false"
echo
echo "Are you building a 32-bit or 64-bit version of the ESMoL toolchain?"
until [ "$valid" = "true" ]; do
  echo -n "Type A for 32-bit, or B for 64-bit: "
  read answer

  modanswer="$(echo "$answer" | tr 'ab' 'AB')"

  if [ "$modanswer" = "A" -o "$modanswer" = "B" ]; then
    valid="true"
  else
    echo "Invalid response \"$answer\""
    echo
  fi
done

arch=i386
if [ "$modanswer" = "B" ]; then
  arch="x86_64"
fi

cat > /etc/yum.repos.d/isis.repo <<EOF
[isis]
name=ISIS 5-$arch
baseurl=http://repo.isis.vanderbilt.edu/Download/UDM/rhel/5/$arch/
enabled=1
gpgcheck=0
EOF

echo "yum isis repository registered."
echo
