; See PATCHES.txt
; See distribution/core.make for info on removing this file.
;
; To update/recreate this module, first run the following command:
;   drush make --no-core build.make
; (answer 'y' to the question, or pass -y in the command)
;
; Then move the resulting module from sites/all/modules/contrib/*
;
; e.g.:
; drush make --no-core build.make -y
; rsync -r --remove-source-files sites/all/modules/contrib/* ..
; find sites/ -type d | sort -r | xargs rmdir

core = 7.x
api = 2

; Reroute Email
projects[reroute_email][type] = module
projects[reroute_email][subdir] = contrib
;
; Fetch release tarball (n.b. produces a better .info file)
projects[reroute_email][download][type] = file
projects[reroute_email][download][url] = "http://ftp.drupal.org/files/projects/reroute_email-7.x-1.x-dev.tar.gz"
;
; Separate the recipient (list) from the address whitelist. Add domain whitelist.: https://drupal.org/comment/7771143#comment-7771143
projects[reroute_email][patch][] = "https://drupal.org/files/whitelists-1571500-26.patch"