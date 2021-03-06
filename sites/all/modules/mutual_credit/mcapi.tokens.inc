<?php

/*
 * implements hook_tokens for a transaction cluster
 * process one transaction at a time, containing dependents
 * field API fields are handled elsewhere
 */
function mcapi_tokens($type, $tokens, array $data = array(), array $options = array()) {
  if ($type != 'transaction') return;
  module_load_include('inc', 'mcapi');
  $transaction = &$data['transaction'];
  $url_options = array('absolute' => TRUE);
  $sanitize = !empty($options['sanitize']);
  $language_code = NULL;

  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  $replacements = array();
  foreach ($tokens as $name => $original) {
    switch ($name) {
      // Simple key values on the node.
      case 'serial':
        $replacements[$original] = $transaction->serial;
        break;
      case 'payer':
        $account = user_load($transaction->payer);
        $replacements[$original] = $sanitize ? filter_xss($account->name) : format_username($account);
        break;
      case 'payee':
        $account = user_load($transaction->payee);
        $replacements[$original] = $sanitize ? filter_xss($account->name) : format_username($account);
        break;
      case 'creator':
        $account = user_load($transaction->creator);
        $replacements[$original] = $sanitize ? filter_xss($account->name) : format_username($account);
        break;
      case 'summary':
        $replacements[$original] = drupal_render(transaction_view($transaction, 'token'));
        break;
      case 'certificate':
        $replacements[$original] = drupal_render(transaction_view(array($transaction), 'certificate'));
        break;
      case 'created':
        $replacements[$original] = format_date($transaction->created, 'medium', '', NULL, $language_code);
        break;
      case 'payer_diff':
      case 'payee_diff':
        $worth = $items = array();
        $diff = _transactions_diff($transaction);
        $uid = $name == 'payer_diff' ? $transaction->payer : $transaction->payer;
        foreach ($diff[$uid] as $currcode => $quantities) {
          $items[] = array('currcode' => $currcode, 'quantity' => array_sum($quantities));
        }
        $element = mcapi_field_formatter_view(NULL, NULL, NULL, NULL, NULL, $items);
        $replacements[$original] = drupal_render($element);
        break;
      case 'links':
        $links = transaction_get_links($transaction);
        foreach (transaction_get_links($transaction) as $link) {
          $ops[] = drupal_render($link);
        }
        $replacements[$original] = implode(' ', $ops);
        break;
      default: //hopefully a field API field
       // drupal_set_message("Unknown token $name");
    }
  }
  return $replacements;
}

function mcapi_token_info() {
  $transaction_tokens = array(
    'serial' => array(
      'name' => t('Unique serial number'),
      'description' => t("The unique serial number of the transaction cluster."),
    ),
    'payer' => array(
      'name' => t('Payer name'),
      'description' => t("The user whose account is credited"),
    ),
    'payee' => array(
      'name' => t('Payee name'),
      'description' => t("The user whose account is debited"),
    ),
    'payer_diff' => array(
      'name' => t("Difference to payer in the main transaction"),
      'description' => t("Total change to payers balance from transaction cluster")
    ),
    'payee_diff' => array(
      'name' => t("Difference to payee in the main transaction"),
      'description' => t("Total change to payees balance from transaction cluster"),
    ),
    'summary' => array(
      'name' => t('Summary'),
      'description' => t("A one-per-line description of the transaction cluster"),
    ),
    'certificate' => array(
      'name' => t('Certificate'),
      'description' => t("An HTML laid out view of the transaction cluster"),
    ),
    'created' => array(
      'name' => t('Created'),
      'description' => t("When the transaction was created"),
    ),
    'creator' => array(
      'name' => t('Creator'),
      'description' => t("The user who first recorded the transaction"),
    ),
    'links' => array(
      'name' => t('Links'),
      'description' => t("Links to transaction operations"),
    )
  );
  return array(
    'types' => array(
      'transaction' => array(
        'name' => t('Transaction'),
        'description' => t('Tokens related to a transaction cluster.'),
        'needs-data' => 'transaction',
      ),
    ),
    'tokens' => array(
      'transaction' => $transaction_tokens,
    ),
  );
}
/*
 * implements hook_token_info_alter
 * remove the 'original' set of tokens, which is added to all entities,
 * but is not necessary, because transactions are not edited
 */
function mcapi_token_info_alter(&$items) {
  unset($items['tokens']['transaction']['original']);

}