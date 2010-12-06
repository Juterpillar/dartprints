Copyright 2010 Josh Waihi http://joshwaihi.com

Description
-----------
The Nagios NRPE module exposes Nagios checks in Drupal to drush to be run by NRPE.

The NRPE addon is designed to allow you to execute Nagios plugins on remote
Linux/Unix machines. The main reason for doing this is to allow Nagios to
monitor "local" resources (like CPU load, memory usage, etc.) on remote
machines. Since these public resources are not usually exposed to external
machines, an agent like NRPE must be installed on the remote Linux/Unix
machines.

If you already use Nagios NRPE in your organization to monitor your infrastructure, then
this module will be useful for you. If you only run one or two Drupal sites, Nagios
may be overkill for this task.

Installation
------------
To enable Nagios NRPE download and read the documentation at
http://nagios.sourceforge.net/docs/nrpe/NRPE.pdf

Configuration for Drupal
------------------------

To enable this module do the following:

1. Go to Admin -> Build -> Modules
   Enable the module.

NOTE: Its recommended to disable Nagios checks via Drupal and
      only use NRPE checks via drush as a security enhancement.

Configuration for Nagios
------------------------
See parent nagios module for details on how to install Nagios
and see http://nagios.sourceforge.net/docs/nrpe/NRPE.pdf for details
on how to setup NRPE checks.

A basic example of checking cron is running: edit the NRPE cfg file and add

  command[drupal_check_cron]=/path/to/drush -r /path/to/drupal nrpe cron

An NRPE check will need to be add at the Nagios server to check for "drupal_check_cron"

Bugs/Features/Patches:
----------------------
If you want to report bugs, feature requests, or submit a patch, please do so
at the project page on the Drupal web site.

Author
------
Josh Waihi (http://joshwaihi.com and http://catalyst.net.nz)

If you use this module, find it useful, and want to send the author
a thank you note, then use the Feedback/Contact page at the URL above.

The author can also be contacted for paid customizations of this
and other modules.

