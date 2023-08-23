<!DOCTYPE html>
<html>
	{include file="head-block.tpl"}
<body>

<script src="/user/js/vendor/jquery-1.12.0.min.js"></script>
<script>
$(document).ready(function() {
	$('dl.sitecomment-report').click(readReport);
});

// this = .sitecomment-report html node jquery object
function readReport() {
	var report = this;

	// post data defaults
	var postData = {
		mode: 'comment',
		action: 'report_read',
		id: 0
	};

	postData.id = $(this).data('report-id');
	// console.log( postData );

	$.post("", postData)
	.done(function(data_, status_, xhr_) {
		if(data_.status=='OK') {
			$(report).attr('data-status', data_.newStatus);
		}
		else {
			alert("server error");
		}
	})
	.fail(function(data_, status_, xhr_) {
		alert("network error");
	});
}
</script>

{foreach $data as $report}
	<dl class="sitecomment-report clearfix" data-status="{$report.status}" data-report-id="{$report.comment_report_id}" >
		<dt class="date">報告日</dt>
		<dd class="date">{$report.date_create|japanesedateFull}</dd>
		<dt class="category">違反カテゴリ</dt>
		<dd class="category">{$report.violation_category|commentReportCategoryName}</dd>
		<dt class="comment">違反内容</dt>
		<dd class="comment">{$report.violation_comment}</dd>
		<dt class="reporter-details">報告者名</dt>
		<dd class="reporter-details">{$report.reporter_name}</dd>
		<dt class="reporter-details">会社</dt>
		<dd class="reporter-details">{$report.reporter_company}</dd>
		<dt class="reporter-details">メールアドレス</dt>
		<dd class="reporter-details">{$report.reporter_email}</dd>
	</dl>
{/foreach}

</body>
</html>
