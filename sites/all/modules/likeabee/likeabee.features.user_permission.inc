<?php
/**
 * @file
 * likeabee.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function likeabee_user_default_permissions() {
  $permissions = array();

  // Exported permission: configure ow.
  $permissions['configure ow'] = array(
    'name' => 'configure ow',
    'roles' => array(
      0 => 'committee',
      1 => 'system',
    ),
    'module' => 'offers_wants',
  );

  // Exported permission: create image content.
  $permissions['create image content'] = array(
    'name' => 'create image content',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: create page content.
  $permissions['create page content'] = array(
    'name' => 'create page content',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: create story content.
  $permissions['create story content'] = array(
    'name' => 'create story content',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any image content.
  $permissions['delete any image content'] = array(
    'name' => 'delete any image content',
    'roles' => array(
      0 => 'committee',
      1 => 'system',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any page content.
  $permissions['delete any page content'] = array(
    'name' => 'delete any page content',
    'roles' => array(
      0 => 'committee',
      1 => 'system',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any story content.
  $permissions['delete any story content'] = array(
    'name' => 'delete any story content',
    'roles' => array(
      0 => 'committee',
      1 => 'system',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own image content.
  $permissions['delete own image content'] = array(
    'name' => 'delete own image content',
    'roles' => array(
      0 => 'authenticated user',
      1 => 'committee',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own page content.
  $permissions['delete own page content'] = array(
    'name' => 'delete own page content',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own story content.
  $permissions['delete own story content'] = array(
    'name' => 'delete own story content',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any image content.
  $permissions['edit any image content'] = array(
    'name' => 'edit any image content',
    'roles' => array(
      0 => 'committee',
      1 => 'system',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any page content.
  $permissions['edit any page content'] = array(
    'name' => 'edit any page content',
    'roles' => array(
      0 => 'committee',
      1 => 'system',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any story content.
  $permissions['edit any story content'] = array(
    'name' => 'edit any story content',
    'roles' => array(
      0 => 'committee',
      1 => 'system',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own image content.
  $permissions['edit own image content'] = array(
    'name' => 'edit own image content',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own page content.
  $permissions['edit own page content'] = array(
    'name' => 'edit own page content',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own story content.
  $permissions['edit own story content'] = array(
    'name' => 'edit own story content',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit propositions.
  $permissions['edit propositions'] = array(
    'name' => 'edit propositions',
    'roles' => array(
      0 => 'committee',
      1 => 'system',
    ),
    'module' => 'offers_wants',
  );

  // Exported permission: post proposition.
  $permissions['post proposition'] = array(
    'name' => 'post proposition',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'offers_wants',
  );

  // Exported permission: view proposition.
  $permissions['view proposition'] = array(
    'name' => 'view proposition',
    'roles' => array(
      0 => 'anonymous user',
      1 => 'authenticated user',
    ),
    'module' => 'offers_wants',
  );

  // Exported permission: create proposition content.
  $permissions['create proposition content'] = array(
    'name' => 'create proposition content',
    'roles' => array(
      0 => 'authenticated user',
      1 => 'trader',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any proposition content.
  $permissions['delete any proposition content'] = array(
    'name' => 'delete any proposition content',
    'roles' => array(
      0 => 'committee',
      1 => 'system',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own proposition content.
  $permissions['delete own proposition content'] = array(
    'name' => 'delete own proposition content',
    'roles' => array(
      0 => 'authenticated user',
      1 => 'committee',
      2 => 'trader',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any proposition content.
  $permissions['edit any proposition content'] = array(
    'name' => 'edit any proposition content',
    'roles' => array(
      0 => 'committee',
      1 => 'system',
    ),
    'module' => 'node',
  );

  // Exported permission: edit documents.
  $permissions['edit documents'] = array(
    'name' => 'edit documents',
    'roles' => array(
      0 => 'committee',
      1 => 'system',
    ),
    'module' => 'cforge_docs',
  );

  // Exported permission: edit own proposition content.
  $permissions['edit own proposition content'] = array(
    'name' => 'edit own proposition content',
    'roles' => array(
      0 => 'authenticated user',
      1 => 'trader',
    ),
    'module' => 'node',
  );

  // Exported permission: view non-private documents.
  $permissions['view non-private documents'] = array(
    'name' => 'view non-private documents',
    'roles' => array(
      0 => 'anonymous user',
    ),
    'module' => 'cforge_docs',
  );

  // Exported permission: view private documents.
  $permissions['view private documents'] = array(
    'name' => 'view private documents',
    'roles' => array(
      0 => 'authenticated user',
      1 => 'trader',
    ),
    'module' => 'cforge_docs',
  );

  return $permissions;
}
