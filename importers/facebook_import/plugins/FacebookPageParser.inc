<?php

/**
 * Extends FeedsExJsonPath to handle filtering / converting feed results before they get processed.
 */
class FacebookPageParser extends FeedsExJsonPath {
	protected $fetcher_fb_id;
	protected $fetcher_fb_page;

	protected function executeContext(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
		$results = parent::executeContext($source, $fetcher_result);
		$fetcher_config = $source->getConfigFor($source->importer->fetcher);
		$this->fetcher_fb_id = $fetcher_config['fb_id'];
		$this->fetcher_fb_page = $fetcher_config['fb_page'];

		$results = array_filter($results, [$this, 'matchesFbFeedId',]);
		$results = array_map([$this, 'convertResultItem',], $results);

		return $results;
	}

	protected function matchesFbFeedId($item) {
		return $item['from']['id'] == $this->fetcher_fb_id;
	}

	protected function convertResultItem($item) {
		// We only want the actual post id, strip out the user id.
		$post_id = explode("_", $item['id']);
		$item['id'] = $post_id[1];

		// Grab the page username from the import form.
		$item['page_username'] = $this->fetcher_fb_page;

		// Construct a post url if it's not a photo, otherwise use photo link.
		if ($item['type'] != 'photo') {
			$item['post_url'] = 'https://www.facebook.com/' . $item['page_username'] . '/posts/' . $item['id'];
		} else {
			$item['post_url'] = $item['link'];
		}

		return $item;
	}
}
