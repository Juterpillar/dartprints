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
; Fetch from repository
; projects[reroute_email][download][type] = git
; projects[reroute_email][download][url] = "http://git.drupal.org/project/reroute_email.git"
; branch HEAD
; projects[reroute_email][download][revision] = "7.x-1.x"
; specific version tag
; projects[reroute_email][download][revision] = "7.x-1.1"
;
; Fetch release tarball (n.b. produces a better .info file)
projects[reroute_email][download][type] = file
projects[reroute_email][download][url] = "http://ftp.drupal.org/files/projects/reroute_email-7.x-1.1.tar.gz"
;
; Separate the recipient (list) from the address whitelist. Add domain whitelist.: http://drupal.org/node/1571500#comment-6006898
projects[reroute_email][patch][] = "http://drupal.org/files/whitelists-1571500-9.patch"
; More robust Cc / Bcc suppression: http://drupal.org/node/1571508#comment-5982972
projects[reroute_email][patch][] = "http://drupal.org/files/cc_bcc-1571508.patch"
; Add email test form: http://drupal.org/node/1571502#comment-5982958
projects[reroute_email][patch][] = "http://drupal.org/files/test_form-1571502.patch"
