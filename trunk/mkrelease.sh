#! /bin/sh

releasefile=release.files

test -f $releasefile || { echo "$0: $releasefile: no such file."; exit 1; }

test $1 || { echo "$0: no version string.\n\tSyntax: $0 version"; exit 1; }

version=$1
tmpdir=`mktemp -d -t panda-player-XXXX`
tmpname=panda_player-$version
outdir=$PWD/releases

echo " ** creating directories"
mkdir -pv "$outdir"
mkdir -pv "$tmpdir"/"$tmpname"
echo " ** copying files to $tmpdir/$tmpname"
for a in `cat $releasefile`; do
	cp "$a" "$tmpdir"/"$tmpname"
done

cd $tmpdir

echo " ** creating $outdir/$tmpname.tar.gz"
tar -czvf "$outdir"/"$tmpname".tar.gz "./$tmpname"
echo " ** deleting temporary files"
rm -r "$tmpdir"
echo " ** all done."
