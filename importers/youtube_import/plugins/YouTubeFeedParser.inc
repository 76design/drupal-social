<?php

/**
 * Extends FeedsExJsonPath to handle converting feed results before they get processed.
 */
class YouTubeFeedParser extends FeedsExJsonPath {

  protected function executeContext(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $results = parent::executeContext($source, $fetcher_result);
    $results = array_map([$this, 'convertResultItem',], $results);

    return $results;
  }

  protected function convertResultItem($item) {
    // Extract ID from ID url.
    $post_id = explode("/", $item['id']['$t']);
    $item['id']['$t'] = end($post_id);
    // Convert dates to timestamps.
    $item['published']['$t'] = strtotime($item['published']['$t']);
    $item['updated']['$t'] = strtotime($item['updated']['$t']);

    return $item;
  }
}
