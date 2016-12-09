<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 *
 * A QUICK OVERVIEW OF DRUPAL THEMING
 *
 *   The default HTML for all of Drupal's markup is specified by its modules.
 *   For example, the comment.module provides the default HTML markup and CSS
 *   styling that is wrapped around each comment. Fortunately, each piece of
 *   markup can optionally be overridden by the theme.
 *
 *   Drupal deals with each chunk of content using a "theme hook". The raw
 *   content is placed in PHP variables and passed through the theme hook, which
 *   can either be a template file (which you should already be familiary with)
 *   or a theme function. For example, the "comment" theme hook is implemented
 *   with a comment.tpl.php template file, but the "breadcrumb" theme hooks is
 *   implemented with a theme_breadcrumb() theme function. Regardless if the
 *   theme hook uses a template file or theme function, the template or function
 *   does the same kind of work; it takes the PHP variables passed to it and
 *   wraps the raw content with the desired HTML markup.
 *
 *   Most theme hooks are implemented with template files. Theme hooks that use
 *   theme functions do so for performance reasons - theme_field() is faster
 *   than a field.tpl.php - or for legacy reasons - theme_breadcrumb() has "been
 *   that way forever."
 *
 *   The variables used by theme functions or template files come from a handful
 *   of sources:
 *   - the contents of other theme hooks that have already been rendered into
 *     HTML. For example, the HTML from theme_breadcrumb() is put into the
 *     $breadcrumb variable of the page.tpl.php template file.
 *   - raw data provided directly by a module (often pulled from a database)
 *   - a "render element" provided directly by a module. A render element is a
 *     nested PHP array which contains both content and meta data with hints on
 *     how the content should be rendered. If a variable in a template file is a
 *     render element, it needs to be rendered with the render() function and
 *     then printed using:
 *       <?php print render($variable); ?>
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 *   The template.php file is one of the most useful files when creating or
 *   modifying Drupal themes. With this file you can do three things:
 *   - Modify any theme hooks variables or add your own variables, using
 *     preprocess or process functions.
 *   - Override any theme function. That is, replace a module's default theme
 *     function with one you write.
 *   - Call hook_*_alter() functions which allow you to alter various parts of
 *     Drupal's internals, including the render elements in forms. The most
 *     useful of which include hook_form_alter(), hook_form_FORM_ID_alter(),
 *     and hook_page_alter(). See api.drupal.org for more information about
 *     _alter functions.
 *
 * OVERRIDING THEME FUNCTIONS
 *
 *   If a theme hook uses a theme function, Drupal will use the default theme
 *   function unless your theme overrides it. To override a theme function, you
 *   have to first find the theme function that generates the output. (The
 *   api.drupal.org website is a good place to find which file contains which
 *   function.) Then you can copy the original function in its entirety and
 *   paste it in this template.php file, changing the prefix from theme_ to
 *   bia_. For example:
 *
 *     original, found in modules/field/field.module: theme_field()
 *     theme override, found in template.php: bia_field()
 *
 *   where bia is the name of your sub-theme. For example, the
 *   zen_classic theme would define a zen_classic_field() function.
 *
 *   Note that base themes can also override theme functions. And those
 *   overrides will be used by sub-themes unless the sub-theme chooses to
 *   override again.
 *
 *   Zen core only overrides one theme function. If you wish to override it, you
 *   should first look at how Zen core implements this function:
 *     theme_breadcrumbs()      in zen/template.php
 *
 *   For more information, please visit the Theme Developer's Guide on
 *   Drupal.org: http://drupal.org/node/173880
 *
 * CREATE OR MODIFY VARIABLES FOR YOUR THEME
 *
 *   Each tpl.php template file has several variables which hold various pieces
 *   of content. You can modify those variables (or add new ones) before they
 *   are used in the template files by using preprocess functions.
 *
 *   This makes THEME_preprocess_HOOK() functions the most powerful functions
 *   available to themers.
 *
 *   It works by having one preprocess function for each template file or its
 *   derivatives (called theme hook suggestions). For example:
 *     THEME_preprocess_page    alters the variables for page.tpl.php
 *     THEME_preprocess_node    alters the variables for node.tpl.php or
 *                              for node--forum.tpl.php
 *     THEME_preprocess_comment alters the variables for comment.tpl.php
 *     THEME_preprocess_block   alters the variables for block.tpl.php
 *
 *   For more information on preprocess functions and theme hook suggestions,
 *   please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/node/223440 and http://drupal.org/node/1089656
 */


/**
 * Override or insert variables into the maintenance page template.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("maintenance_page" in this case.)
 */
/* -- Delete this line if you want to use this function
function bia_preprocess_maintenance_page(&$variables, $hook) {
  // When a variable is manipulated or added in preprocess_html or
  // preprocess_page, that same work is probably needed for the maintenance page
  // as well, so we can just re-use those functions to do that work here.
  bia_preprocess_html($variables, $hook);
  bia_preprocess_page($variables, $hook);
}
// */

/**
 * Override or insert variables into the html templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("html" in this case.)
 */
//* -- Delete this line if you want to use this function
function bia_preprocess_html(&$variables, $hook) {
  //$variables['sample_variable'] = t('Lorem ipsum.');

  // The body tag's classes are controlled by the $classes_array variable. To
  // remove a class from $classes_array, use array_diff().
  //$variables['classes_array'] = array_diff($variables['classes_array'], array('class-to-remove'));
  $node = menu_get_object();

  //if (in_array('section-shop', $variables['classes_array'])) $variables['classes_array'][] = 'layout-shop';
  //if (in_array('section-cart', $variables['classes_array'])) $variables['classes_array'][] = 'layout-shop';
  //if (in_array('section-checkout', $variables['classes_array'])) $variables['classes_array'][] = 'layout-shop';
  

  if (isset($node) && (isset($node->field_layout[LANGUAGE_NONE][0]['value']))) {
    $variables['classes_array'][]= 'layout-'.$node->field_layout[LANGUAGE_NONE][0]['value'];
  }
  
  if (isset($node) && (isset($node->type))) {
    switch ($node->type) {
    	case 'webform':
    	case 'article':
    	$variables['classes_array'][] = 'whiteheader'; ;
    	break;
    	default:
    	;
    	break;
    }
  } else if ($variables['menu_item']['page_callback'] == 'views_page') {
    switch ($variables['menu_item']['page_arguments'][0]) {
    	case 'fashion_types':
    	case 'learn_from_the_best':
    	case 'commerce_user_orders':
    	case '_addressbook':
        case 'faq':
    	 $variables['classes_array'][] = 'whiteheader'; ;
    	break;
    }
    
    switch ($variables['menu_item']['page_arguments'][0]) {
    	case 'product_listing_nailiners':
    	  $variables['classes_array'][] = 'product-listings'; ;
    	  break;
    }
    
  } else {
    $variables['classes_array'][] = 'whiteheader'; ;
  }
  
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
//* -- Delete this line if you want to use this function
function bia_preprocess_page(&$variables, $hook) {
  $bia_colors = array();
  $i = 0;
  while ($color = theme_get_setting('bia_color_'.++$i)) $bia_colors[] = '#'.$color;
  if (count($bia_colors)) drupal_add_js(array('bia_colors'=>$bia_colors),'setting');
  
  global $user;

  if ($_GET['q'] == 'user/login' || $_GET['q'] == 'user' && !$user->uid) {
    drupal_set_title('Login');
  }
}
// */

/**
 * Override or insert variables into the node templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
/* -- Delete this line if you want to use this function
function bia_preprocess_node(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');

  // Optionally, run node-type-specific preprocess functions, like
  // bia_preprocess_node_page() or bia_preprocess_node_story().
  $function = __FUNCTION__ . '_' . $variables['node']->type;
  if (function_exists($function)) {
    $function($variables, $hook);
  }
}
// */

/**
 * Override or insert variables into the comment templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function bia_preprocess_comment(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the region templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("region" in this case.)
 */
/* -- Delete this line if you want to use this function
function bia_preprocess_region(&$variables, $hook) {
  // Don't use Zen's region--sidebar.tpl.php template for sidebars.
  //if (strpos($variables['region'], 'sidebar_') === 0) {
  //  $variables['theme_hook_suggestions'] = array_diff($variables['theme_hook_suggestions'], array('region__sidebar'));
  //}
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function bia_preprocess_block(&$variables, $hook) {
  // Add a count to all the blocks in the region.
  // $variables['classes_array'][] = 'count-' . $variables['block_id'];

  // By default, Zen will use the block--no-wrapper.tpl.php for the main
  // content. This optional bit of code undoes that:
  //if ($variables['block_html_id'] == 'block-system-main') {
  //  $variables['theme_hook_suggestions'] = array_diff($variables['theme_hook_suggestions'], array('block__no_wrapper'));
  //}
}
// */

function bia_menu_link($variables){
  $element = $variables ['element'];
  $sub_menu = '';
  
  if ($element ['#below']) {
    $sub_menu = drupal_render ( $element ['#below'] );
  }
  
  $element ['#localized_options']['html'] = true;
  $element['#title'] ='<span class="link-inner">'.$element['#title'].'</span><span class="corner1"></span><span class="corner2"></span><span class="corner3"></span><span class="corner4"></span>';
  
  $output = l ( $element ['#title'], $element ['#href'], $element ['#localized_options'] );
  return '<li' . drupal_attributes ( $element ['#attributes'] ) . '>' . $output . $sub_menu . "</li>\n";
}


function bia_form_element_label($variables) {
  $element = $variables['element'];
  // This is also used in the installer, pre-database setup.
  $t = get_t();

  // If title and required marker are both empty, output no label.
  if ((!isset($element['#title']) || $element['#title'] === '') && empty($element['#required'])) {
    return '';
  }

  // If the element is required, a required marker is appended to the label.
  $required = !empty($element['#required']) ? theme('form_required_marker', array('element' => $element)) : '';

  $title = filter_xss_admin($element['#title']);

  $attributes = array();
  // Style the label as class option to display inline with the element.
  if ($element['#title_display'] == 'after') {
    $attributes['class'] = 'option';
  }
  // Show label only to screen readers to avoid disruption in visual flows.
  elseif ($element['#title_display'] == 'invisible') {
    $attributes['class'] = 'element-invisible';
  }

  if (!empty($element['#id'])) {
    $attributes['for'] = $element['#id'];
  }

  // The leading whitespace helps visually separate fields from inline labels.
  return ' <label' . drupal_attributes($attributes) . '>' . $t('!title !required', array('!title' => $title, '!required' => $required)) . "</label>\n";
}


function bia_field($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<div class="field-label"' . $variables['title_attributes'] . '>' . $variables['label'] . '</div>';
  }

  // Render the items.
  $output .= '<div class="field-items"' . $variables['content_attributes'] . '>';
  foreach ($variables['items'] as $delta => $item) {
    $classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
    $output .= '<div class="' . $classes . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</div>';
  }
  $output .= '</div>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';

  return $output;
}

function bia_field__field_layout($variables){

  $output = '';
  foreach ($variables['items'] as $delta => $item) {
    $variables['classes'] .= ' layout-'.drupal_render($item).' ';
  }
  $output .= '';
  
  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';
  
  return $output;
}

function bia_field__field_reverse_columns_flow_on_mo($variables){

  $output = '';
  foreach ($variables['items'] as $delta => $item) {
    $variables['classes'] .= ' flexrow-'.drupal_render($item).' ';
  }
  $output .= '';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '></div>';

  return $output;
  
}

function bia_field__field_youtube($variables){
  foreach ($variables['element']['#items'] as $id=>$item) {
    $variables['items'][$id]['#markup'] ='<iframe width="640" height="360" src="https://www.youtube.com/embed/'.$item['safe_value'].'" frameborder="0" allowfullscreen></iframe>'; 
  } 
  return bia_field($variables);  
}

function bia_commerce_price_formatted_components($variables){
  foreach ($variables['components'] as $price_name=>$price) {
    if (isset($price['suffix'])) {
      $price['formatted_price'].=' / '.$price['suffix'];
    }
    $price['title'] = t($price['title']);
    $variables['components'][$price_name] = $price;
  }
  // Add the CSS styling to the table.
  drupal_add_css(drupal_get_path('module', 'commerce_price') . '/theme/commerce_price.theme.css');

  // Build table rows out of the components.
  $rows = array();

  foreach ($variables['components'] as $name => $component) {
    $rows[] = array(
      'data' => array(
        array(
          'data' => $component['title'],
          'class' => array('component-title'),
        ),
        array(
          'data' => $component['formatted_price'],
          'class' => array('component-total'),
        ),
      ),
      'class' => array(drupal_html_class('component-type-' . $name)),
    );
  }

  return theme('table', array('rows' => $rows, 'attributes' => array('class' => array('commerce-price-formatted-components', 'table-style-1'))));
}

function bia_fieldset($variables){
  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper'));

  $output = '<fieldset' . drupal_attributes($element['#attributes']) . '>';
  if (!empty($element['#title'])) {
    // Always wrap fieldset legends in a SPAN for CSS positioning.
    $output .= '<legend><span class="fieldset-legend">' . t($element['#title']) . '</span></legend>';
    $output .= '<span class="fieldset-legend-alternative">' . t($element['#title']) . '</span>';
  }
  $output .= '<div class="fieldset-wrapper">';
  if (!empty($element['#description'])) {
    $output .= '<div class="fieldset-description">' . $element['#description'] . '</div>';
  }
  $output .= $element['#children'];
  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }
  $output .= '</div>';
  $output .= "</fieldset>\n";
  return $output;
}

function bia_form($variables) {
  $element = $variables['element'];
  if (isset($element['#action'])) {
    $element['#attributes']['action'] = drupal_strip_dangerous_protocols($element['#action']);
  }
  element_set_attributes($element, array('method', 'id'));
  if (empty($element['#attributes']['accept-charset'])) {
    $element['#attributes']['accept-charset'] = "UTF-8";
  }
  // Anonymous DIV to satisfy XHTML compliance.
  return '<form' . drupal_attributes($element['#attributes']) . '><div>' . $element['#children'] . '</div></form>';
}

function bia_file_link($variables){
	return theme_file_link($variables);
}

function bia_links__locale_block($variables){
	foreach ($variables['links'] as $lang=>$lnk) {
		if (!isset($lnk['href'])) $variables['links'][$lang]['href']='<front>';
		$variables['links'][$lang]['title'] = $lang;
		}
	return theme_links($variables);
}

function bia_select($variables) {
  $element = $variables['element'];
  if (isset($element['#exposedall']) && $element['#exposedall']) {
  	$title = $element['#exposedall'];
  	$element['#options']['All'] = $title;
  	$element['#attributes']['class'][] = 'first-opt-label';
  }
  
  element_set_attributes($element, array('id', 'name', 'size'));
  _form_set_class($element, array('form-select'));
  return '<div class="select-widget"><select' . drupal_attributes($element['#attributes']) . '>' . form_select_options($element) . '</select><input type="text" class="select-text form-text" tabindex="-1" readonly="readonly" style="display:none" /></div>';
}

function bia_textfield($variables){
  $element = $variables['element'];
  if (isset($variables['element']['#title']) && !isset($variables['element']['#attributes']['placeholder'])) $variables['element']['#attributes']['placeholder'] = $variables['element']['#title'];
  return theme_textfield($variables);
}

function bia_password($variables){
  $element = $variables['element'];
  if (!isset($variables['element']['#attributes']['placeholder'])) $variables['element']['#attributes']['placeholder'] = $variables['element']['#title'];
  return theme_password($variables);
}

function bia_textarea($variables){
  $element = $variables['element'];
  if (!isset($variables['element']['#attributes']['placeholder'])) $variables['element']['#attributes']['placeholder'] = $variables['element']['#title'];
  return theme_textarea($variables);
}

function bia_select_as_radios($vars) {
  if ($vars['element']['#name'] == 'type') {
    $element = &$vars['element'];
    
    if (!empty($element['#bef_nested'])) {
      return theme('select_as_tree', $vars);
    }
    
    //dpm($element);
    
    $output = '';
    foreach (element_children($element) as $key) {
      $element[$key]['#default_value'] = NULL;
      $term = taxonomy_term_load($element[$key]['#return_value']);
      
      $row = '';
      
      if (isset($term->field_svg[LANGUAGE_NONE][0])) {
        $row.= '<img class="nailiner" alt="'.(isset($term->field_svg[LANGUAGE_NONE][0]['description'])?$term->field_svg[LANGUAGE_NONE][0]['description']:'').'" src="'.file_create_url($term->field_svg[LANGUAGE_NONE][0]['uri']).'">';
      }
      
      if (isset($term->field_svg_hover[LANGUAGE_NONE][0])) {
        $row.= '<img class="nailiner-hover" alt="'.(isset($term->field_svg[LANGUAGE_NONE][0]['description'])?$term->field_svg[LANGUAGE_NONE][0]['description']:'').'" src="'.file_create_url($term->field_svg_hover[LANGUAGE_NONE][0]['uri']).'">';
      }
      $row.= '<span class="ntext">'.$term->name.'</span>'; 
      //dpm($term);
      
      
      $element[$key]['#children'] = theme('radio', array('element' => $element[$key])).$row;
      $output .= theme('form_element', array('element' => $element[$key]));
    }
    
    return $output;
    
  } else {
    return theme_select_as_radios($vars);
  }
  
}

function bia_preprocess_views_exposed_form(&$vars) {
  $form = &$vars['form'];
  foreach ($form['#info'] as $id => $info) {
    $widget = $vars['widgets'][$id];
    if ($form[$info['value']]['#type'] == 'select') {
      $form[$info['value']]['#printed'] = false;
      $form[$info['value']]['#exposedall'] = $info['label'];
      $widget->widget = drupal_render($form[$info['value']]);
      $vars['widgets'][$id] = $widget;
    }
  }
}

function bia_field__field_product_pictures($variables){
  $view = views_get_view('_product_pictures');
  $view->hide_admin_links = TRUE;
  $display_id = 'default';
  $view->set_display($display_id);
  $view->preview = TRUE; //avoid lots of unuseful menu hooks title hooks etc
  $view->pre_execute(array($variables['element']['#object']->nid)); // args passed here
  $view->is_cacheable = FALSE; // optionally
  return $view->render($display_id);
}


function bia_field__file($variables){
  foreach($variables['items'] as $pos => $item) {
    if ($item['#file']->filemime == 'image/svg+xml') {
      $variables['items'][$pos]['#theme'] = 'image';
      $variables['items'][$pos]['#item'] = array(
          'path' => $item['#file']->uri,
          'uri' => $item['#file']->uri,
          'width' => '',
          'height' => '',
          'alt' => $item['#file']->description,
          'title' => $item['#file']->description,
      );
      
      $variables['items'][$pos]['#path'] = $item['#file']->uri; 
    }
  }
  
  return bia_field($variables);
}

function bia_field__field_color($variables) {
  foreach ($variables['element']['#items'] as $delta => $item) {
    if (!isset($variables['item_attributes'][$delta])) $variables['item_attributes'][$delta] = '';
    $variables['item_attributes'][$delta].= ' style="background-color: #'.$item['jquery_colorpicker'].'" ';
  }
  
  return bia_field($variables);
}

/* function bia_pager_first($variables){
  $text = $variables['text'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  global $pager_page_array;
  $output = '';

  // If we are anywhere but the first page
  if ($pager_page_array[$element] > 0) {
    $output = theme('pager_link', array('text' => $text, 'page_new' => pager_load_array(0, $element, $pager_page_array), 'element' => $element, 'parameters' => $parameters));
  } else {
    $output = theme('pager_link', array('text' => $text, 'page_new' => pager_load_array(0, $element, $pager_page_array), 'element' => $element, 'parameters' => $parameters));
  }

  return $output;
} */
  

function bia_block_view_alter(&$data, $block) {
  global $user;
  if (!$user->uid && $block->module=='user' && $block->delta == 'login') {
    $data['subject'] = t('User login');
    $data['content'] = drupal_get_form('user_login_block');
  }
}


function bia_form_alter(&$form, &$form_state, $form_id) {
  if($form_id == 'user_register_form') {
    $form['actions']['back_login'] = array(
    	'#type' =>  'markup',
        '#markup'=>  l(t('Back to login page'),'user/login',array('attributes'=>array('class'=>array('fancybutton')))),
    );

  }
}


function bia_field__commerce_order_total($variables){
  $variables['classes'].=' table-style-1';
  return bia_field($variables);
}


function bia_commerce_checkout_progress_list($variables) {
  $path = drupal_get_path('module', 'commerce_checkout_progress');
  drupal_add_css($path . '/commerce_checkout_progress.css');

  extract($variables);

  // Option to display back pages as links.
  if ($link) {
    if ($order = menu_get_object('commerce_order')) {
      $order_id = $order->order_id;
    }
    // Load the *shopping cart* order. It gets deleted on last page.
    elseif (module_exists('commerce_cart') && $order = commerce_cart_order_load($GLOBALS['user']->uid)) {
      $order_id = $order->order_id;
    }
  }

  // This is where we build up item list that will be themed
  // This variable is used with $variables['link'], see more in inside comment.
  $visited = TRUE;
  // Our list of progress pages.
  $progress = array();
  foreach ($items as $page_id => $page) {
    $class = array();
    if ($page_id === $current_page) {
      $class[] = 'active';
      // Active page and next pages should not be linked.
      $visited = FALSE;
    }
    if (isset($items[$current_page]['prev_page']) && $page_id === $items[$current_page]['prev_page']) {
      $class[] = 'previous';
    }
    if (isset($items[$current_page]['next_page']) && $page_id === $items[$current_page]['next_page']) {
      $class[] = 'next';
    }
    $class[] = $page_id;
    $data = t($page['title']);

    if ($visited) {
      $class[] = 'visited'; // Issue #1345942.

      // On checkout complete page, the checkout order is deleted.
      if (isset($order_id) && $order_id) {
        // If a user is on step 1, clicking a link next steps will be redirect them back.
        // Only render the link on the pages those user has already been on.
        // Make sure the loaded order is the same one found in the URL.
        if (arg(1) == $order_id) {
          $href = isset($page['href']) ? $page['href'] : "checkout/{$order_id}/{$page_id}";
          $data = l(filter_xss($data), $href, array('html' => TRUE));
        }
      }
    }
    
    if (!isset($order_id) || !$order_id || arg(1) != $order_id) {
      $data = '<a '.($page_id === $current_page?'class="active"':'').'><span class="link-inner">'.$data.'</span><span class="corner1"></span><span class="corner2"></span><span class="corner3"></span><span class="corner4"></span></a>';
    }

    $item = array(
        'data'  => $data,
        'class' => $class,
    );
    // Only set li title if the page has help text.
    if (isset($page['help'])) {
      //#1322436 Filter help text to be sure it contains NO html.
      $help = strip_tags($page['help']);
      // Make sure help has text event after filtering html.
      if (!empty($help)) {
        $item['title'] = $help;
      }
    }
    // Add item to progress array.
    $progress[] = $item;
  }

  $classes = array(
      'commerce-checkout-progress',
      'menu',
      'checkout-pages-' . count($progress),
  );
  return theme('item_list', array(
      'items' => $progress,
      'type' => $type,
      'attributes' => array('class' => $classes),
  ));
}

function bia_checkbox($variables){
  return '<span class="checkbox-widget">'.theme_checkbox($variables).'<span class="checkbox-widget-bg"></span></span>';
}

function bia_pager_first($variables){
  $text = $variables['text'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  global $pager_page_array;
  $output = '';
  $attributes = array();

  if ($pager_page_array[$element] > 0) {
    //
  } else {
    $attributes['href'] = "";
  }
  
  $output = theme('pager_link', array('text' => $text, 'page_new' => pager_load_array(0, $element, $pager_page_array), 'element' => $element, 'parameters' => $parameters, 'attributes' => $attributes));
  return $output;
}

function bia_pager_last($variables){
  $text = $variables['text'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  global $pager_page_array, $pager_total;
  $output = '';
  $attributes = array();

  // If we are anywhere but the last page
  if ($pager_page_array[$element] < ($pager_total[$element] - 1)) {
    //
  } else {
    $attributes['href'] = "";
  }
    
  $output = theme('pager_link', array('text' => $text, 'page_new' => pager_load_array($pager_total[$element] - 1, $element, $pager_page_array), 'element' => $element, 'parameters' => $parameters, 'attributes' => $attributes));

  return $output;
}

function bia_pager_previous($variables){
  $text = $variables['text'];
  $element = $variables['element'];
  $interval = $variables['interval'];
  $parameters = $variables['parameters'];
  global $pager_page_array;
  $output = '';
  $attributes = array();

  // If we are anywhere but the first page
  if ($pager_page_array[$element] > 0) {
    $page_new = pager_load_array($pager_page_array[$element] - $interval, $element, $pager_page_array);

    // If the previous page is the first page, mark the link as such.
    if ($page_new[$element] == 0) {
      $output = theme('pager_first', array('text' => $text, 'element' => $element, 'parameters' => $parameters, 'attributes' => $attributes));
    }
    // The previous page is not the first page.
    else {
      $output = theme('pager_link', array('text' => $text, 'page_new' => $page_new, 'element' => $element, 'parameters' => $parameters, 'attributes' => $attributes));
    }
  } else {
    $attributes['href'] = "";
    $output = theme('pager_first', array('text' => $text, 'element' => $element, 'parameters' => $parameters, 'attributes' => $attributes));
  }

  return $output;
}

function bia_pager_next($variables){
  $text = $variables['text'];
  $element = $variables['element'];
  $interval = $variables['interval'];
  $parameters = $variables['parameters'];
  global $pager_page_array, $pager_total;
  $output = '';
  $attributes = array();
  
  // If we are anywhere but the last page
  if ($pager_page_array[$element] < ($pager_total[$element] - 1)) {
    $page_new = pager_load_array($pager_page_array[$element] + $interval, $element, $pager_page_array);
    // If the next page is the last page, mark the link as such.
    if ($page_new[$element] == ($pager_total[$element] - 1)) {
      $output = theme('pager_last', array('text' => $text, 'element' => $element, 'parameters' => $parameters, 'attributes' => $attributes));
    }
    // The next page is not the last page.
    else {
      $output = theme('pager_link', array('text' => $text, 'page_new' => $page_new, 'element' => $element, 'parameters' => $parameters, 'attributes' => $attributes));
    }
  } else {
    $attributes['href'] = "";
    $output = theme('pager_last', array('text' => $text, 'element' => $element, 'parameters' => $parameters, 'attributes' => $attributes));
  }
  
  return $output;
}

function bia_pager($variables){
  $tags = $variables['tags'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $quantity = $variables['quantity'];
  global $pager_page_array, $pager_total;

  // Calculate various markers within this pager piece:
  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);
  // current is the page we are currently paged to
  $pager_current = $pager_page_array[$element] + 1;
  // first is the first page listed by this pager piece (re quantity)
  $pager_first = $pager_current - $pager_middle + 1;
  // last is the last page listed by this pager piece (re quantity)
  $pager_last = $pager_current + $quantity - $pager_middle;
  // max is the maximum page number
  $pager_max = $pager_total[$element];
  // End of marker calculations.

  // Prepare for generation loop.
  $i = $pager_first;
  if ($pager_last > $pager_max) {
    // Adjust "center" if at end of query.
    $i = $i + ($pager_max - $pager_last);
    $pager_last = $pager_max;
  }
  if ($i <= 0) {
    // Adjust "center" if at start of query.
    $pager_last = $pager_last + (1 - $i);
    $i = 1;
  }
  // End of generation loop preparation.

  $li_first = theme('pager_first', array('text' => (isset($tags[0]) ? $tags[0] : t('« first')), 'element' => $element, 'parameters' => $parameters));
  $li_previous = theme('pager_previous', array('text' => (isset($tags[1]) ? $tags[1] : t('‹ previous')), 'element' => $element, 'interval' => 1, 'parameters' => $parameters));
  $li_next = theme('pager_next', array('text' => (isset($tags[3]) ? $tags[3] : t('next ›')), 'element' => $element, 'interval' => 1, 'parameters' => $parameters));
  $li_last = theme('pager_last', array('text' => (isset($tags[4]) ? $tags[4] : t('last »')), 'element' => $element, 'parameters' => $parameters));

  if ($pager_total[$element] > 1) {
    if ($li_first) {
      $items[] = array(
        'class' => array('pager-first'),
        'data' => $li_first,
      );
    }
    if ($li_previous) {
      $items[] = array(
        'class' => array('pager-previous'),
        'data' => $li_previous,
      );
    }

    // When there is more than one page, create the pager list.
    if ($i != $pager_max) {
      if ($i > 1) {
        $items[] = array(
          'class' => array('pager-ellipsis'),
          'data' => '…',
        );
      }
      // Now generate the actual pager piece.
      for (; $i <= $pager_last && $i <= $pager_max; $i++) {
        if ($i < $pager_current) {
          $items[] = array(
            'class' => array('pager-item'),
            'data' => theme('pager_previous', array('text' => $i, 'element' => $element, 'interval' => ($pager_current - $i), 'parameters' => $parameters)),
          );
        }
        if ($i == $pager_current) {
          $items[] = array(
            'class' => array('pager-current'),
            'data' => theme('pager_link', array('text' => $i, 'element' => $element, 'interval' => ($pager_current - $i), 'parameters' => $parameters, 'attributes' => array('href'=>"", 'title' => t('Current page')))),
          );
        }
        if ($i > $pager_current) {
          $items[] = array(
            'class' => array('pager-item'),
            'data' => theme('pager_next', array('text' => $i, 'element' => $element, 'interval' => ($i - $pager_current), 'parameters' => $parameters)),
          );
        }
      }
      if ($i < $pager_max) {
        $items[] = array(
          'class' => array('pager-ellipsis'),
          'data' => '…',
        );
      }
    }
    // End generation.
    if ($li_next) {
      $items[] = array(
        'class' => array('pager-next'),
        'data' => $li_next,
      );
    }
    if ($li_last) {
      $items[] = array(
        'class' => array('pager-last'),
        'data' => $li_last,
      );
    }
    return '<h2 class="element-invisible">' . t('Pages') . '</h2>' . theme('item_list', array(
      'items' => $items,
      'attributes' => array('class' => array('pager')),
    ));
  }
  
}



function bia_pager_link($variables){
  $text = $variables['text'];
  $page_new = $variables['page_new'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $attributes = $variables['attributes'];

  $page = isset($_GET['page']) ? $_GET['page'] : '';
  if ($new_page = implode(',', pager_load_array($page_new[$element], $element, explode(',', $page)))) {
    $parameters['page'] = $new_page;
  }

  $query = array();
  if (count($parameters)) {
    $query = drupal_get_query_parameters($parameters, array());
  }
  if ($query_pager = pager_get_query_parameters()) {
    $query = array_merge($query, $query_pager);
  }

  // Set each pager link title
  if (!isset($attributes['title'])) {
    static $titles = NULL;
    if (!isset($titles)) {
      $titles = array(
        t('« first') => t('Go to first page'),
        t('‹ previous') => t('Go to previous page'),
        t('next ›') => t('Go to next page'),
        t('last »') => t('Go to last page'),
      );
    }
    if (isset($titles[$text])) {
      $attributes['title'] = $titles[$text];
    }
    elseif (is_numeric($text)) {
      $attributes['title'] = t('Go to page @number', array('@number' => $text));
    }
  }

  // @todo l() cannot be used here, since it adds an 'active' class based on the
  //   path only (which is always the current path for pager links). Apparently,
  //   none of the pager links is active at any time - but it should still be
  //   possible to use l() here.
  // @see http://drupal.org/node/1410574
  if (!isset($attributes['href'])) {
   
    $attributes['href'] = url($_GET['q'], array('query' => $query));
  } elseif ($attributes['href'] == "") {
    unset($attributes['href']);
  }
  return '<a' . drupal_attributes($attributes) . '>' . check_plain($text) . '</a>';
}





