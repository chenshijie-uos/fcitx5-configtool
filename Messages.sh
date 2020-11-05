#!/bin/bash
function gen_pot() {
DOMAIN=$1
shift 1
POT_FILE=po/$DOMAIN/$DOMAIN.pot
set -x
XGETTEXT="xgettext --package-name=$DOMAIN --add-comments --sort-output --msgid-bugs-address=fcitx-dev@googlegroups.com"
source_files=$(find "$@" -name \*.cpp -o -name \*.h -o -name \*.qml)
$XGETTEXT --keyword=_ --keyword=C_:1c,2 --keyword=N_ --kde --language=C++ -o ${POT_FILE} $source_files
desktop_files=$(find "$@" -name \*.conf.in -o -name \*.conf.in.in -o -name \*.desktop.in)
$XGETTEXT --language=Desktop -k --keyword=Name --keyword=GenericName --keyword=Comment --keyword=Keywords $desktop_files -j -o ${POT_FILE}
ui_files=$(find "$@" -name \*.ui)
extractrc $ui_files > rc.cpp
$XGETTEXT --kde --language=C++ --add-comments --sort-output -j -o ${POT_FILE} rc.cpp
rm -f rc.cpp

sed -i 's|^"Content-Type: text/plain; charset=CHARSET\\n"|"Content-Type: text/plain; charset=utf-8\\n"|g' ${POT_FILE}

# Due to transifex problem, delete the date.
#sed -i '/^"PO-Revision-Date/d' ${POT_FILE}
#sed -i '/^"PO-Revision-Date/d' ${POT_FILE}
sed -i '/^# FIRST AUTHOR/d' ${POT_FILE}
sed -i '/^#, fuzzy/d' ${POT_FILE}
sed -i 's|^"Language: \\n"|"Language: LANG\\n"|g' ${POT_FILE}

echo > po/$DOMAIN/LINGUAS

for pofile in $(ls po/$DOMAIN/*.po | sort); do
  pofilebase=$(basename $pofile)
  pofilebase=${pofilebase/.po/}
  msgmerge -U --backup=none $pofile ${POT_FILE}
  project_line=$(grep "Project-Id-Version" ${POT_FILE} | head -n 1 | tr --delete '\n' | sed -e 's/[\/&]/\\&/g')
  sed -i "s|.*Project-Id-Version.*|$project_line|g" $pofile
  echo $pofilebase >> po/$DOMAIN/LINGUAS
done

}

gen_pot fcitx5-configtool layout/ src/configtool/ src/lib/ src/migrator/
gen_pot org.fcitx.fcitx5.kcm src/kcm/
