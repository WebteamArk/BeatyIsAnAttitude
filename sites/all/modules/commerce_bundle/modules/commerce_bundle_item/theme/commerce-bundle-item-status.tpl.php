<?php

/**
 * @file
 * Default theme implementation to present the status on a bundle item page.
 *
 * Available variables:
 * - $status: The string representation of a item's status to render.
 * - $label: If present, the string to use as the status label.
 *
 * Helper variables:
 * - $entity: The fully loaded entity object the status belongs to.
 */
?>
<?php if ($status): ?>
  <div class="commerce-bundle-item-status">
    <?php if ($label): ?>
      <span class="commerce-bundle-item-status-label">
        <?php print $label; ?>
      </span>
    <?php endif; ?>
    <?php print $status; ?>
  </div>
<?php endif; ?>
