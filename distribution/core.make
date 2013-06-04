core = 7.x
api = 2
projects[ckeditor][subdir] = "contrib"
projects[ctools][subdir] = "contrib"
projects[date][subdir] = "contrib"
projects[email][subdir] = "contrib"
projects[entity][subdir] = "contrib"
projects[entitycache][subdir] = "contrib"
projects[entityreference][subdir] = "contrib"
projects[environment][subdir] = "contrib"
projects[features][subdir] = "contrib"
projects[field_group][subdir] = "contrib"
projects[flag][subdir] = "contrib"
projects[globalredirect][subdir] = "contrib"
projects[libraries][subdir] = "contrib"
projects[link][subdir] = "contrib"
projects[pathauto][subdir] = "contrib"
projects[redirect][subdir] = "contrib"
projects[rules][subdir] = "contrib"
projects[securepages][subdir] = "contrib"
projects[strongarm][subdir] = "contrib"
projects[token][subdir] = "contrib"
projects[token_tweaks][subdir] = "contrib"
projects[transliteration][subdir] = "contrib"
projects[views][subdir] = "contrib"
projects[views_bulk_operations][subdir] = "contrib"

;; Once https://drupal.org/node/2011284 is committed, or we figure out a way of
;; having different directories for modules and themes we can use this singular
;; drush make file (and delete the existing build.make in each project)
;; for the entire upstream repo. Until then we must maintain the
;; build.make files for each project.
;;
;; Pathced contrib
;;
;; Reroute Email
;projects[reroute_email][type] = module
;projects[reroute_email][download][type] = file
;projects[reroute_email][download][url] = "http://ftp.drupal.org/files/projects/reroute_email-7.x-1.1.tar.gz"
;projects[reroute_email][destination] = "../../modules/catalyst"
;; Separate the recipient (list) from the address whitelist. Add domain whitelist.: http://drupal.org/node/1571500#comment-6006898
;projects[reroute_email][patch][] = "http://drupal.org/files/whitelists-1571500-9.patch"
;; More robust Cc / Bcc suppression: http://drupal.org/node/1571508#comment-5982972
;projects[reroute_email][patch][] = "http://drupal.org/files/cc_bcc-1571508.patch"
;; Add email test form: http://drupal.org/node/1571502#comment-5982958
;projects[reroute_email][patch][] = "http://drupal.org/files/test_form-1571502.patch"
;
;; Entity cache
;projects[entitycache][type] = module
;projects[entitycache][subdir] = contrib
;projects[entitycache][version] = 1.x-dev
;; PDOException:Invalid text representation when attempting to load an entity with a string ID
;; http://drupal.org/node/1851398
;projects[entitycache][patch][] = "http://drupal.org/files/entitycache-numeric-ids.patch"
;
;; Rubik
;projects[rubik][type] = theme
;projects[rubik][destination] = "../../themes/catalyst"
;projects[rubik][version] = 4.x-dev
;; Support Error classes on table rows https://drupal.org/node/1879560
;projects[rubik][patch][] = "https://drupal.org/files/rubik-table-error-striping.patch"
;; Add an option to display form field descriptions inline (rather than using pop-ups) https://drupal.org/node/1730844
;projects[rubik][patch][] = "http://drupal.org/files/rubik_inline_field_desc-1730844-10.patch"
;; Fix fieldsets inside vertical tabs https://drupal.org/node/1307826
;projects[rubik][patch][] = "https://drupal.org/files/rubik-fieldset-in-fieldset-display-1307826-5.patch"