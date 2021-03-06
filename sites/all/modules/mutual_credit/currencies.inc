<?php

function _mcapi_currency_form(&$form, &$form_state, $currency) {
  $form['#tree'] = TRUE;
  $form['info']['currcode']['#maxlength'] = 8;
  $form['human_name'] = array(
    '#title' => t('Name of currency'),
    '#description' => t('Use the plural'),
    '#type' => 'textfield',
    '#default_value' => $currency->human_name,
    '#weight' => -1,
  );
  //need to do a live machine name preview here, like views does
  //http://api.drupal.org/api/drupal/includes%21form.inc/function/form_process_machine_name/7

  $form['display'] = array(
    '#title' => t('Appearance'),
    '#type' => 'fieldset',
    '#weight' => 5
  );
  $form['display']['format'] = array(
    '#title' => t('Display format'),
    '#description' => t('Write an expression to control the display of the currency using html, css and the following tokens:') .'@minus, @integer, @subdivision.' .' '.
       t('For example, !a shows as !b', array('!a' => '<em>@minus@integer:@subdivision AUS</em>', '!b' => '<em>-99:99 AUS</em>')),
    '#type' => 'textfield',
    '#default_value' => $currency->display['format'],
    '#element_validate' => array('mcapi_currency_validate_format'),
    '#required' => TRUE,
    '#weight' => 1
  );
  $form['display']['divisions'] = array(
    '#title' => t('Subdivisions'),
    '#type' => 'select',
    '#options' => array(
      CURRENCY_DIVISION_MODE_NONE => t('Integer values only'),
      CURRENCY_DIVISION_MODE_CENTS_INLINE => t("Cents in same field, delimited by '.'"),
      CURRENCY_DIVISION_MODE_CENTS_FIELD => t('Cents in separate field'),
      CURRENCY_DIVISION_MODE_CUSTOM => t('Allowed subdivisions, below')
    ),
    '#default_value' => $currency->display['divisions'],
    '#weight' => 2
  );
  $form['display']['divisions_setting'] = array(
    '#title' => t('Allowed subdivisions'),
    '#description' => t('On each line put "hundredths| visible text". The visible text replaces the @subdivision in the display format, above.'),
    '#type' => 'textarea',
    '#cols' => 60,
    '#rows' => 4,
    '#resizable' => FALSE,
    '#default_value' => isset($currency->display['divisions_setting']) ? $currency->display['divisions_setting'] : "0|/.\n25|1/4\n50|1/2\n75|3/4",
    '#element_validate' => array('mcapi_currency_validate_divisions'),
    '#weight' => 3
  );

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 10,
  );
  $form['currency_access'] = array(
    '#title' => 'Currency access',
    '#type' => 'fieldset',
    '#group' => 'additional_settings',
    '#weight' => -1
  );
  $weight = 0;


  foreach (module_implements('permission') as $module) {
    $function = $module .'_permission';
    foreach ($function() as $perm => $info) {
      $options[$module][$perm] = strip_tags($info['title']);
    }
  }
  $form['currency_access']['membership'] = array(
    '#title' => t('Use the currency'),
    '#description' => t('Determine which users are permitted to use this currency'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => @$currency->currency_access['membership'] ? $currency->currency_access['membership'] : 'transact',
    '#weight' => $weight++,
  );
  $form['currency_access']['user_aggregated_view'] = array(
    '#title' => t('View aggregated user transaction data'),
    '#description' => t("Such as users' balances, gross income, number of transactions"),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => @$currency->currency_access['user_aggregated_view'] ? $currency->currency_access['user_aggregated_view'] : 'transact',
    '#weight' => $weight++,
  );
  $form['currency_access']['currency_aggregated_view'] = array(
    '#title' => t('View aggregated system data'),
    '#description' => t('Look at currency usage stats stripped of personal information'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => @$currency->currency_access['currency_aggregated_view'] ? $currency->currency_access['currency_aggregated_view'] : 'transact',
    '#weight' => $weight++,
  );

  static $i = 0;
  static $j = 0;
  $form['transaction_view_access'] = array(
    '#title' => t('Privacy'),
    '#description' => t('Determine who can view transactions in each state.') .' '. t('Any the checked conditions must return TRUE'),
    '#type' => 'fieldset',
    '#group' => 'additional_settings',
  );
  $access_callbacks = module_invoke_all('transaction_access_callbacks');
  foreach (mcapi_get_states('#full') as $constant => $state) {
    $states = isset($currency->view) ? $currency->view : array();
    $form['transaction_view_access'][$constant] = array(
      '#title' => t("Transactions in state '@state'", array('@state' => $state['name'])),
      '#description' => $state['description'],
      '#type' => 'checkboxes',
      '#options' => $access_callbacks,
      '#default_value' => isset($currency->transaction_view_access[$constant]) ? $currency->transaction_view_access[$constant] : array(),
      '#weight' => $j++,
    );
  }
  $form['transaction_operation_access'] = array(
    '#title' => t('Transaction operations'),
    '#description' => t('Determine who can do what to transactions') .'. '. t('Any of the checked conditions must return TRUE'),
    '#type' => 'fieldset',
    '#group' => 'additional_settings',
  );

  foreach (module_invoke_all('transaction_operations') as $callback => $info) {
    $form['transaction_operation_access'][$callback] = array(
      '#title' => $info['description'],
      '#description' => @$info['description'],
      '#type' => 'checkboxes',
      '#options' => $access_callbacks,
      '#default_value' => isset($currency->transaction_operation_access[$callback]) ? $currency->transaction_operation_access[$callback] : array(),
      '#weight' => $j++,
    );
  }

  $form['uid'] = array(
    '#title' => t('Declared by'),
    '#description' => t("Users with permission to 'transact'"),
    '#type' => 'user_chooser_permissions',
    '#args' => array('transact'),
    '#default_value' => $currency->uid ? $currency->uid : $GLOBALS['user']->name,
    '#allow_me' => TRUE,
    '#required' => TRUE,
    '#weight' => 4,
  );

  if (isset($currency->currcode)) {
    $form['buttons']['submit']['#value'] = t('Update');
  }
}


function mcapi_currency_validate_format(&$element, $form_state) {
  if (!strpos($element['#value'], '@integer')) {
    form_error($element, t("Currency format must contain token '@integer'"));
  }
  //we also need to check that the format field contains the divisions token
  if ($form_state['values']['display']['divisions'] != CURRENCY_DIVISION_MODE_NONE && !strpos($element['#value'], '@subdivision')) {
    form_error($element, t("Currency format must contain token '@subdivision'"));
  }
}

function mcapi_currency_validate_divisions(&$element, $form_state) {
  if ($form_state['values']['display']['divisions'] != CURRENCY_DIVISION_MODE_CUSTOM) return;
  $allowed = $element['#value'];
  $lines = explode("\n", $allowed);
  if (count($lines) < 2) {
    form_error($element, t("There should be at least two lines in field '@fieldname'", array('@fieldname' => $element['#title'])));
  }
  foreach ($lines as $line) {
    if (strpos($line, '|') === FALSE) {
      form_error($element, t('line "@val" should contain a pipe character, |', array('@val' => $line)));
    }
    $vals = explode('|', $line);
    if (!is_numeric($vals[0]) || !strlen($lines[0]) || $vals[1] != check_plain($vals[1]) || !strlen($vals[1] || !is_integer($vals[1]))) {
      form_error($element,
        t("'@val' should be an integer from  0 to 99, followed directly by a pipe, |, followed directly by a word or phrase with no unusual characters",
          array('@val' => $line)
        )
      );
    }
  }
}

/*
 * Validate callback for transaction form 'currcode' element
 */
function mcapi_validate_currcode(&$element, $form_state) {
  if (isset($element['#value']) && $element['#value']) {
    $currency = currency_load($element['#value']);
    if (!is_object($currency)) { //you would also check here if the user had permission to use this currency
      form_error($element, t('Invalid currency specified: @num', array('@num' => $element['#value'])));
    }
  }
}
/*
 * menu access callback
 * access to the ctools currency edit page
 */
function currency_edit_access($currency) {
  return user_access('configure all currencies') || $currency->uid == $GLOBALS['user']->uid;
}
/*
 * load the currencies from ctools and alter them before they get used
 */
function currencies_load_all() {
  ctools_include('export');
  $currencies = array();
  $functions = array_keys(module_invoke_all('transaction_access_callbacks'));
  $all = ctools_export_crud_load_all('mcapi_currencies');
  //unpack them - this is a bit of a shortcut, but ctools takes you right round the park
  foreach($all as $currcode => $currency) {
    if (!empty($currency->disabled)) continue;
    $data = &$currency->data;
    //convert the divisions_allowed into an array
    $divisions = array();
    if ($data->display['divisions'] == CURRENCY_DIVISION_MODE_CUSTOM) {
      foreach(explode("\n", $data->display['divisions_setting']) as $line) {
        list($cent, $display) = explode('|', $line);
        $divisions[$cent] = $display;
      }
    }
    $data->display['divisions_allowed'] = $divisions;
    //this is a good moment to check that all the currencies for all the operations, for all the states, have access callbacks
    $accesses = array('transaction_view_access' => 'Privacy', 'transaction_operation_access' => 'Transaction operations', 'currency_access' => 'Currency Access');
    foreach ($accesses as $prop => $name) {
      if (property_exists($data, $prop)) continue;
      drupal_set_message(
        "Resave currency $data->human_name, paying attention to $name : ". l(t('edit'), 'admin/accounting/currencies/list/'.$currcode.'/edit'),
        'warning'
      );
    }
    $currencies[$currcode] = $currency;
  }
  return $currencies;
}
//load the default currency and clear some fields, so it can populate the edit form with some discrete defaults
function default_cur() {
  $currency = ctools_export_crud_load('mcapi_currencies', 'def_drup')->data;
  $currency->human_name = '';
  $currency->display['format'] = '@minus@integer @subdivision';
  return $currency;
}

/*
 * Validate callback for mcapi_currency_form
 * declared in plugins/export/mcapi_ui
 * set the $form_state['item'] for ctools to save
 */
function mcapi_currency_form_validate(){

}
/*
 * Submit callback for mcapi_currency_form
 * declared in plugins/export/mcapi_ui
 * set the $form_state['item'] for ctools to save
 */
function mcapi_currency_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  //things got a little out of kilter here - this shouldn't be necessary
  $form_state['item']->currcode = $form_state['values']['info']['currcode'];
  $form_state['item']->data = (object)$form_state['values'];
  cache_clear_all('currencies', 'cache');
}