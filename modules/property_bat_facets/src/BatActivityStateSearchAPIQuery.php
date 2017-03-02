<?php

/**
 * @file
 */

/**
 *
 */
class BatActivityStateSearchAPIQuery implements FacetapiQueryTypeInterface {

  /**
   * Implements FacetapiQueryTypeInterface::getType().
   */
  public static public function getType() {
    return 'bat_activity_state';
  }

  /**
   * Implements FacetapiQueryTypeInterface::execute().
   */
  public function execute($query) {
  }

  /**
   * Implements FacetapiQueryTypeInterface::build().
   *
   * Unlike normal facets, we provide a static list of options.
   */
  public function build() {
    $build = array();
    return $build;
  }

}
