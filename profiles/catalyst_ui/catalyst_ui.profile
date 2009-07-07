<?php
// $Id: default.profile,v 1.22 2007/12/17 12:43:34 goba Exp $

/**
 * Return an array of the modules to be enabled when this profile is installed.
 *
 * @return
 *   An array of modules to enable.
 */
function catalyst_ui_profile_modules() {
  $modules = array(
    // Core Drupal modules.
    'comment', 'help', 'menu', 'taxonomy', 'dblog', 'path', 'upload',
    // Contributed modules
    'admin_menu', 'advanced_help', 
      // Views
      'views', 'views_ui', 
      // Image
      'image', 'imageapi', 'imagecache', 'img_assist',
      // CCK
      'content', 'text', 'filefield', 'imagefield', 
  );

  return $modules;
}

/**
 * Return a description of the profile for the initial installation screen.
 *
 * @return
 *   An array with keys 'name' and 'description' describing this profile,
 *   and optional 'language' to override the language selection for
 *   language-specific profiles.
 */
function catalyst_ui_profile_details() {
  return array(
    'name' => 'Catalyst UI Drupal',
    'description' => 'Acquia enchanced Drupal with additional UI modules for Catalyst project developement. Addition modules include modules for Views, CCK and Image.'
  );
}

/**
 * Return a list of tasks that this profile supports.
 *
 * @return
 *   A keyed array of tasks the profile will perform during
 *   the final stage. The keys of the array will be used internally,
 *   while the values will be displayed to the user in the installer
 *   task list.
 */
function catalyst_ui_profile_task_list() {
  // This is the only profile method that is invoked before the first page is
  // displayed during the install sequence.  Use this opportunity to theme the
  // install experience.
  global $conf;
  $conf['theme_settings'] = array(
    'default_logo' => 0,
    'logo_path' => 'profiles/catalyst_ui/CatalystLogo.png');
  $conf['site_name'] = 'Catalyst Drupal';
}

/**
 * Perform any final installation tasks for this profile.
 *
 * The installer goes through the profile-select -> locale-select
 * -> requirements -> database -> profile-install-batch
 * -> locale-initial-batch -> configure -> locale-remaining-batch
 * -> finished -> done tasks, in this order, if you don't implement
 * this function in your profile.
 *
 * If this function is implemented, you can have any number of
 * custom tasks to perform after 'configure', implementing a state
 * machine here to walk the user through those tasks. First time,
 * this function gets called with $task set to 'profile', and you
 * can advance to further tasks by setting $task to your tasks'
 * identifiers, used as array keys in the hook_profile_task_list()
 * above. You must avoid the reserved tasks listed in
 * install_reserved_tasks(). If you implement your custom tasks,
 * this function will get called in every HTTP request (for form
 * processing, printing your information screens and so on) until
 * you advance to the 'profile-finished' task, with which you
 * hand control back to the installer. Each custom page you
 * return needs to provide a way to continue, such as a form
 * submission or a link. You should also set custom page titles.
 *
 * You should define the list of custom tasks you implement by
 * returning an array of them in hook_profile_task_list(), as these
 * show up in the list of tasks on the installer user interface.
 *
 * Remember that the user will be able to reload the pages multiple
 * times, so you might want to use variable_set() and variable_get()
 * to remember your data and control further processing, if $task
 * is insufficient. Should a profile want to display a form here,
 * it can; the form should set '#redirect' to FALSE, and rely on
 * an action in the submit handler, such as variable_set(), to
 * detect submission and proceed to further tasks. See the configuration
 * form handling code in install_tasks() for an example.
 *
 * Important: Any temporary variables should be removed using
 * variable_del() before advancing to the 'profile-finished' phase.
 *
 * @param $task
 *   The current $task of the install system. When hook_profile_tasks()
 *   is first called, this is 'profile'.
 * @param $url
 *   Complete URL to be used for a link or form action on a custom page,
 *   if providing any, to allow the user to proceed with the installation.
 *
 * @return
 *   An optional HTML string to display to the user. Only used if you
 *   modify the $task, otherwise discarded.
 */
function catalyst_ui_profile_tasks(&$task, $url) {
  global $language;
  
  // Insert default user-defined node types into the database. For a complete
  // list of available node type attributes, refer to the node type API
  // documentation at: http://api.drupal.org/api/HEAD/function/hook_node_info.
  $types = array(
    array(
      'type' => 'page',
      'name' => st('Page'),
      'module' => 'node',
      'description' => st("A <em>page</em>, similar in form to a <em>story</em>, is a simple method for creating and displaying information that rarely changes, such as an \"About us\" section of a website. By default, a <em>page</em> entry does not allow visitor comments and is not featured on the site's initial home page."),
      'custom' => TRUE,
      'modified' => TRUE,
      'locked' => FALSE,
      'help' => '',
      'min_word_count' => '',
    ),
    array(
      'type' => 'story',
      'name' => st('Story'),
      'module' => 'node',
      'description' => st("A <em>story</em>, similar in form to a <em>page</em>, is ideal for creating and displaying content that informs or engages website visitors. Press releases, site announcements, and informal blog-like entries may all be created with a <em>story</em> entry. By default, a <em>story</em> entry is automatically featured on the site's initial home page, and provides the ability to post comments."),
      'custom' => TRUE,
      'modified' => TRUE,
      'locked' => FALSE,
      'help' => '',
      'min_word_count' => '',
    ),
  );

  foreach ($types as $type) {
    $type = (object) _node_type_set_defaults($type);
    node_type_save($type);
  }

  // Default page to not be promoted and have comments disabled.
  variable_set('node_options_page', array('status'));
  variable_set('comment_page', COMMENT_NODE_DISABLED);

  // Don't display date and author information for page nodes by default.
  $theme_settings = variable_get('theme_settings', array());
  $theme_settings['toggle_node_info_page'] = FALSE;
  variable_set('theme_settings', $theme_settings);
  
  // Add a node describing how to get started with catalyst Drupal
  $welcome_file = 'profiles/catalyst_ui/' . $language->language . '/welcome.txt';
  if (!file_exists($welcome_file)) {
    drupal_set_message(t('Could not find file !filepath', array('!filepath' => $welcome_file)), 'error');
    $welcome_file = 'profiles/catalyst_ui/en/welcome.txt';
  }
  if ($welcome_node = _catalyst_ui_profile_create_node($welcome_file, 'page')) {
    node_save($welcome_node);
    variable_set('catalyst_ui_welcome', $welcome_node->nid);
  } else {
    drupal_set_message(t('The file !filepath could not be read.  The welcome
      message will not be generated.',
      array('!filepath' => $welcome_file)), 'error');
  }

  // Use the catalyst profile when running simpletest.
  variable_set('web_test_case_profile', 'catalyst_ui');

  // Update the menu router information.
  menu_rebuild();
}

/**
 * Implementation of hook_form_alter().
 *
 * Allows the profile to alter the site-configuration form. This is
 * called through custom invocation, so $form_state is not populated.
 */
function catalyst_ui_form_alter(&$form, $form_state, $form_id) {
  if ($form_id == 'install_configure') {
    // Set default for site name field.
    $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];

    // We need to provide original submission handler as well, or
    // it would not be called otherwise.
    $form['#submit'] = array('install_configure_form_submit');
    
    // set to any value but screen scaper build tools to enable testing setups
    // like enabling simpletest
    $form['testing'] = array(
      '#type' => 'hidden');
  }
}

/**
 * Creates a node from the specified filename.  The node body will contain the
 * contents of the file.  All relative links must be identified within the file
 * so they can be mapped to paths appropriate for the installation.
 * 
 * The relative links are identified in the file by surrounding the actual
 * link with double square brackets.  For example:
 * 
 * <a href="[[admin]]">Admin page</a> 
 * 
 * $filename the name of the file to retrieve the text from
 * $page_type the type of node to create
 */
function _catalyst_ui_profile_create_node($filename, $page_type) {
  $contents = trim(file_get_contents($filename));
  if (!$contents) {
    return null;
  }

  // Grab the title from the document and then remove the title so it
  // isn't shown in the title and the body.  The title will be encoded
  // in the document in the following form:
  // <h1 [class="..."]>{TITLE}</h1>
  $title_regexp = "/[\<]h1(\s*[^=\>]*=\"[^\"]*\")*\s*[\>](.*)\<\/h1\>/i"; 
  if (preg_match($title_regexp, $contents, $match)) {
      $title = $match[count($match) - 1];
      // Remove the title from the body of the document.
      $contents = preg_replace($title_regexp, '', $contents);
  }
  
  // Replace all local links with the full path.
  $options = array();
  $options['alias'] = TRUE;
  $link_regexp = "/(\[\[)([\/?\w+\-*]+)(\]\])/e";
  $contents = preg_replace($link_regexp, 'check_url(url("\2", $options))',
    $contents);
  $node = new stdClass();
  $node->title = $title;
  $node->teaser = node_teaser($contents);
  $node->body = $contents;
  $node->uid = 1;
  $node->type = $page_type;
  $node->status = 1;
  $node->promote = 1;
  $node->format = 2;
  return $node;
}

