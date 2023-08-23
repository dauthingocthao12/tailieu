<div class="comment-rejected">
    <div class="comment-reject-title"><i class="fa fa-exclamation-triangle"></i> 管理者からのメッセージ</div>
    {foreach $rejected as $reject}
    <div class="comment-reject-date">{$reject.date_create|japanesedate}</div>
    <div class="comment-reject-mail">{$reject.mail_content}</div>
    {/foreach}
</div>