
# create the updating package for arm
tools/packer -P JH5058 -C pack.cfg -o release/update.pkg

#rm -rf firmware*

echo "+----------------------------------------------------+"
echo "|   finished.                                        |"
echo "+----------------------------------------------------+"

exit 1
;;
