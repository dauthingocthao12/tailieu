<a name="comment-{$comment.site_comment_id}"></a>
<div id="comment-{$comment.site_comment_id}" class="comment-area clearfix" data-id="{$comment.site_comment_id}">
	<div class="table-cell situation">
		<dl>
			<dt>状態</dt>
			<dd>
				{$comment.status|commentStatusJapanese}
				{if $comment.status == 'published'}
					{* hide button *}
					<span class="function-icon"><a href='{sitelink mode="account/comment/hide/{$comment.site_comment_id}"}' class="btn btn-xs btn-danger"><i class="fa fa-eye-slash"></i></a></span>

					<div>
						<a href="{siteLink mode="site-description/{$comment.site_id}/page{$comment|findUserCommentPage}"}#comment-{$comment.site_comment_id}">自分のコメントを見る</a>
					</div>
				{/if}
				{if $comment.status == 'hidden'}
					{* show button *}
					<span class="function-icon"><a href='{sitelink mode="account/comment/show/{$comment.site_comment_id}"}' class="btn btn-xs btn-success"><i class="fa fa-eye"></i></a></span>
				{/if}
			</dd>
			<dt>投稿日</dt>
			<dd>{$comment.date_create|japanesedate}</dd>
			{if $is_review!=true}
			<dt>サイト名</dt>
			<dd><a href="{$comment|siteLinkDecide}" target="_blank">{$comment.site_name} <i class="fa fa-external-link"></i></a></dd>
			{/if}
		</dl>
	</div>
	<div class="talk table-cell">
		<div><b>評価</b></div>
		<div class="hyouka">{insert siteEvaluationStars evaluation=$comment.evaluation}　</div>
		<div>
			<b>コメント</b>
			{if $comment.status!='pending' && $is_review!=true}
				{* edit button *}
				<span class="function-icon"><a href="{sitelink mode="site-description"}{$comment.site_id}#edit-my-comment" class="btn btn-xs btn-primary"><i class="fa fa-pencil"></i></a></span>
			{/if}
		</div>

		<div class="talk-text">{$comment.comment|nl2br}</div>

		{if $rejected[$comment.site_comment_id]}
			{include "site-comment-rejected.part.tpl" rejected=$rejected[$comment.site_comment_id]}
		{/if}

		{if $comment.status == 'published'}
			<div class="comment-bottom">
				{* いいね *}
				<span class="function-icon"><i class="fa fa-heart"></i> <span class="heart-count">{$comment.likes_count}</span></span>
			</div>
		{/if}

	</div>
</div>
