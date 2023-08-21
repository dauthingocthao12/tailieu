{extends file="main.tpl"}
{block name=body}

<script src="/user/js/vendor/jquery-1.12.0.min.js"></script>
<script>
$(document).ready(function() {

	// フォームをサブミットする前に、設定を確認
	$('#comment-admin-form').submit(function(e_) {

		// form input
		var comment_status_published = $('[name=status][value=published]').is(':checked');
		// console.log( comment_status_published );
		// return false;
		var email_flag_yes = $("#mail_sent_yes", this).is(':checked');
		var email_input = $("[name=mail_content]", this);

		// check flag
		var can_submit = true;

		// メールデータがありますが、送るかどうか知らない場合
		if(email_input.val() && !email_flag_yes) {
			//　エラーメッセージ
			alert("メール内容が書いていますが、送るボタンが設定されていません。");
			can_submit = false;
		}

		if(email_input.val() && email_flag_yes) {
			// 確認メッセージ
			can_submit = confirm("このメール内容をユーザに送りますか？");
		}

		if(!email_input.val() && email_flag_yes && !comment_status_published) {
			alert("メールを送るボタンが設定されていますが、メール内容がありません。");
			can_submit = false;
		}

		return can_submit;
	});

	// 公開設定時に、メール内容をdisableにする
	$('input[name=status]').change(function() {
		if($(this).val()=='published') {
			$('textarea[name=mail_content]').attr('disabled', true).val('');
			$('select[name=mail_content_template]').attr('disabled', true);
			// $('.mail-select label, .mail-select input').attr('disabled', true);
		}
		else {
			$('textarea[name=mail_content]').attr('disabled', false);
			$('select[name=mail_content_template]').attr('disabled', false);
			// $('.mail-select label, .mail-select input').attr('disabled', false);
		}
	});

	// メールのコンテンツのテンプレート
	var mail_templates = [
		"",
		"{$admin_comment_msg_templates|join:'","'}"
	];

	$('[name=mail_content_template]').change(function() {
		// console.log("Boo!");
		var template_index = $(this).val();
		var template = mail_templates[template_index];
		$('[name=mail_content]').val(template);
	});

	// コメントの違反報告表示ボタンのアクション
	$('a#comment_report_listing').click(function(e_){
		e_.preventDefault();
		var listing = window.open("about:blank", "reports", "width=500, height=800");
		$('form#comment_report_listing_action').submit();
	});

});
</script>

<div class="text-right">
	{makelink mode="comment" action="" value="一覧へ戻る" class="btn btn-warning"}
</div>
<div class="clear">&nbsp;</div>

<form action="" id="comment_report_listing_action" target="reports" method="post">
	<input type="hidden" name="mode" value="comment">
	<input type="hidden" name="action" value="report_listing">
	<input type="hidden" name="listing_filter" value="all"> <!-- とりあえず未使用 -->
	<input type="hidden" name="id" value="{$data.site_comment_id}">
</form>

<div class="sitecomment-detail">
	<form id="comment-admin-form" action="" method="POST">
		<input type="hidden" name="mode" value="comment">
		<input type="hidden" name="action" value="save">
		<input type="hidden" name="id" value="{$data.site_comment_id}">
		<input type="hidden" name="comment_content" value="{$data.comment}">

		{if $message}
		<div class="alert alert-{$message.status}">{$message.content}</div>
		{/if}

		<table class="table table-bordered table-sitecomment-details">
			<tr>
				<th>サイトID</th>
				<td>{$data.site_id}</td>
				<th>サイト名</th>
				<td>{$data.site_name}</td>
			</tr>
			<tr>
				<th>ユーザID</th>
				<td>{$data.user_id}</td>
				<th>ユーザのハンドルネーム</th>
				<td>{$data.handlename}</td>
			</tr>
			<tr>
				<th>メールアドレス</th>
				<td><a href="mailto:{$data.user_email}">{$data.user_email}</a></td>
				<th>受信設定</th>
				<td>
					{if $data.user_notification_comment_published==1}<i class="fa fa-fw fa-check"></i>{else}<i class="fa fa-fw fa-times"></i>{/if} 公開受信<br>
					{if $data.user_notification_comment_rejected==1}<i class="fa fa-fw fa-check"></i>{else}<i class="fa fa-fw fa-times"></i>{/if} 無効受信
				</td>
			</tr>
		</table>

		<table class="table table-bordered table-sitecomment-details">
			<tr>
				<th>公開設定</th>
				<td colspan="2">
					<label><input type="radio" value="pending" name="status" {if $data.status == "pending"}checked="1"{/if}> {"pending"|commentStatusJapanese}</label>
					<label><input type="radio" value="published" name="status" {if $data.status == "published"}checked="1"{/if}> {"published"|commentStatusJapanese}</label>
					<label><input type="radio" value="rejected" name="status" {if $data.status == "rejected"}checked="1"{/if}> {"rejected"|commentStatusJapanese}</label>
				</td>
			</tr>
			<tr>
				<th>投稿状況</th>
				<td colspan="2">{if $data.parent_revision}変更{else}新規{/if}</td>
			</tr>

			<tr>
				<th>投稿日</th>
				<td>{$data.date_create|japanesedateFull}</td>
				<td class="parent_revision">{$data.revision.date_create|japanesedateFull}</td>
			</tr>

			<tr>
				<th>評価</th>
				<td>{insert siteEvaluationStars evaluation=$data.evaluation}</td>
				<td class="parent_revision">{insert siteEvaluationStars evaluation=$data.revision.evaluation}</td>
			</tr>
			<tr>
				<th>コメント {if $data.is_reported}<a href="#comment-reports" id="comment_report_listing" class="btn btn-xs btn-warning"><i class="fa fa-fw fa-flag"></i> 違反報告を見る</a>{/if}</th>
				<td>{$data.comment|nl2br}</td>
				<td class="parent_revision">{$data.revision.comment|nl2br}</td>
			</tr>

			<tr>
				<th>メール内容</th>
				<td colspan="2">
					<select name="mail_content_template" class="form-control" {if $data.status=='published'}disabled='disabled'{/if}>
						<option value="0">テンプレートなし</option>
						{foreach $admin_comment_msg_templates as $v}
						<option value="{$v@iteration}">{$v@key}</option>
						{/foreach}
					</select>
					<br>
					<textarea class="form-control" name="mail_content" rows="10" {if $data.status=='published'}disabled='disabled'{/if} placeholder="メールの送信内容です"></textarea>
					<div class="mail-select">
						<label><input type="checkbox" id="mail_sent_yes" name="mail_sent" value="yes"> 送信する</label>
					</div>
					<div class="alert alert-info">
						公開するときに、「自動お知らせメール」を送るために、上記のボタンをチェックしてください。<br>
						この場合は、メールのメッセージがいりません。
					</div>
				</td>
			</tr>
			<tr>
				<th>備考</th>
				<td colspan="2">
					<textarea class="form-control" name="memo" rows="5" placeholder="このユーザーさんはブラックかも…？">{$data.admin_memo}</textarea>
				</td>
			</tr>
			<tr>
				<th>審査決定日</th>
				<td colspan="2">
					{if $data.admin_date}
						{$data.admin_date}
					{else}
						無し
					{/if}
				</td>
			</tr>
		</table>

		<div class="text-center">
			<input type="submit" value="確定" class="btn btn-primary">
		</div>

	</form>

	<div class="admin-history">
		{foreach $data.history as $history}
		<div class="history-entry">
			<table>
				<tr>
					<td class="left-column">
						<dl>
							<dt><i class="fa fa-calendar"></i> 日時</dt>
							<dd>{$history.date_create}</dd>
						</dl>
					</td>
					<td>
						<dl>
							<dt><i class="fa fa-comment"></i> ユーザのコメント内容</dt>
							<dd>{$history.comment_content|nl2br}</dd>
						</dl>
					</td>
				</tr>
				<tr>
					<td>
						<dl>
							<dt><i class="fa fa-info-circle"></i> アクション</dt>
							<dd>{$history.action|nl2br}</dd>
						</dl>
					</td>
					<td>
						{if $history.mail_sent}
						<dl>
							<dt><i class="fa fa-envelope"></i> メール内容</dt>
							<dd>{$history.mail_content|nl2br}</dd>
						</dl>
						{else}
						&nbsp;
						{/if}
					</td>
				</tr>
			</table>
		</div>
		{foreachelse}
		<div class="alert alert-info">履歴がありません</div>
		{/foreach}
	</div>
</div>
{/block}
