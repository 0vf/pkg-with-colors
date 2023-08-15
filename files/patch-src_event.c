--- src/event.c.orig	2023-08-01 08:25:14 UTC
+++ src/event.c
@@ -62,6 +62,12 @@
 #include "pkgcli.h"
 
 #define STALL_TIME 5
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
+#define COLOR_BLUE "\033[34m"
 
 xstring *messages = NULL;
 xstring *conflicts = NULL;
@@ -98,7 +104,7 @@ cleanup_handler(int dummy __unused)
 
 	if (tll_length(cleanup_list) == 0)
 		return;
-	warnx("\nsignal received, cleaning up");
+	warnx(COLOR_YELLOW BOLD "\nsignal received, cleaning up" RESET);
 	tll_foreach(cleanup_list, it) {
 		ev = it->item;
 		ev->cb(ev->data);
@@ -172,7 +178,7 @@ job_status_begin(xstring *msg)
 	}
 
 	if ((nbtodl > 0 || nbactions > 0) && nbdone > 0)
-		fprintf(msg->fp, "[%d/%d] ", nbdone, (nbtodl) ? nbtodl : nbactions);
+		fprintf(msg->fp, BOLD COLOR_BLUE "[%d/%d] " RESET BOLD, nbdone, (nbtodl) ? nbtodl : nbactions, RESET);
 	if (nbtodl > 0 && nbtodl == nbdone) {
 		nbtodl = 0;
 		nbdone = 0;
@@ -211,7 +217,7 @@ event_sandboxed_call(pkg_sandbox_cb func, int fd, void
 		}
 		if (WIFSIGNALED(status)) {
 			/* Process got some terminating signal, hence stop the loop */
-			fprintf(stderr, "Sandboxed process pid=%d terminated abnormally by signal: %d\n",
+			fprintf(stderr, COLOR_RED BOLD "Sandboxed process pid=%d terminated abnormally by signal: %d\n" RESET,
 					(int)pid, WTERMSIG(status));
 			ret = -1;
 		}
@@ -316,7 +322,7 @@ event_sandboxed_get_string(pkg_sandbox_cb func, char *
 		}
 		if (WIFSIGNALED(status)) {
 			/* Process got some terminating signal, hence stop the loop */
-			fprintf(stderr, "Sandboxed process pid=%d terminated abnormally by signal: %d\n",
+			fprintf(stderr, COLOR_RED BOLD "Sandboxed process pid=%d terminated abnormally by signal: %d\n" RESET,
 					(int)pid, WTERMSIG(status));
 			ret = -1;
 		}
@@ -499,9 +505,9 @@ draw_progressbar(int64_t current, int64_t total)
 				stalled = 0;
 
 			if (stalled >= STALL_TIME)
-				printf(" - stalled -");
+				printf(COLOR_YELLOW BOLD " - stalled -" RESET);
 			else if (bytes_per_second == 0 && bytes_left > 0)
-				printf("   --:-- ETA");
+				printf(COLOR_YELLOW BOLD "   --:-- ETA" RESET);
 			else {
 				if (bytes_left > 0)
 					seconds = bytes_left / bytes_per_second;
@@ -514,13 +520,13 @@ draw_progressbar(int64_t current, int64_t total)
 				seconds -= minutes * 60;
 
 				if (hours != 0)
-					printf("%02d:%02d:%02d", hours,
+					printf(COLOR_YELLOW BOLD "%02d:%02d:%02d" RESET, hours,
 					    minutes, seconds);
 				else
-					printf("   %02d:%02d", minutes, seconds);
+					printf(COLOR_YELLOW BOLD "   %02d:%02d" RESET, minutes, seconds);
 
 				if (bytes_left > 0)
-					printf(" ETA");
+					printf(COLOR_YELLOW BOLD " ETA" RESET);
 				else
 					printf("    ");
 			}
@@ -557,30 +563,30 @@ event_callback(void *data, struct pkg_event *ev)
 
 	switch(ev->type) {
 	case PKG_EVENT_ERRNO:
-		warnx("%s(%s): %s", ev->e_errno.func, ev->e_errno.arg,
+		warnx(COLOR_RED BOLD "%s(%s): %s" RESET, ev->e_errno.func, ev->e_errno.arg,
 		    strerror(ev->e_errno.no));
 		break;
 	case PKG_EVENT_ERROR:
-		warnx("%s", ev->e_pkg_error.msg);
+		warnx(COLOR_RED BOLD "%s" RESET, ev->e_pkg_error.msg);
 		break;
 	case PKG_EVENT_NOTICE:
 		if (!quiet)
-			printf("%s\n", ev->e_pkg_notice.msg);
+			printf(BOLD "%s\n" RESET, ev->e_pkg_notice.msg);
 		break;
 	case PKG_EVENT_DEVELOPER_MODE:
-		warnx("DEVELOPER_MODE: %s", ev->e_pkg_error.msg);
+		warnx(COLOR_YELLOW BOLD "DEVELOPER_MODE: %s", ev->e_pkg_error.msg);
 		break;
 	case PKG_EVENT_UPDATE_ADD:
 		if (quiet || !isatty(STDOUT_FILENO))
 			break;
-		printf("\rPushing new entries %d/%d", ev->e_upd_add.done, ev->e_upd_add.total);
+		printf(BOLD "\rPushing new entries " COLOR_BLUE BOLD "%d/%d" RESET, ev->e_upd_add.done, ev->e_upd_add.total);
 		if (ev->e_upd_add.total == ev->e_upd_add.done)
 			printf("\n");
 		break;
 	case PKG_EVENT_UPDATE_REMOVE:
 		if (quiet || !isatty(STDOUT_FILENO))
 			break;
-		printf("\rRemoving entries %d/%d", ev->e_upd_remove.done, ev->e_upd_remove.total);
+		printf(BOLD "\rRemoving entries " COLOR_BLUE BOLD "%d/%d" RESET, ev->e_upd_remove.done, ev->e_upd_remove.total);
 		if (ev->e_upd_remove.total == ev->e_upd_remove.done)
 			printf("\n");
 		break;
@@ -601,7 +607,7 @@ event_callback(void *data, struct pkg_event *ev)
 		}
 		job_status_begin(msg_buf);
 		progress_debit = true;
-		fprintf(msg_buf->fp, "Fetching %s", filename);
+		fprintf(msg_buf->fp, BOLD "Fetching %s" RESET, filename);
 		break;
 	case PKG_EVENT_FETCH_FINISHED:
 		progress_debit = false;
@@ -612,7 +618,7 @@ event_callback(void *data, struct pkg_event *ev)
 		job_status_begin(msg_buf);
 
 		pkg = ev->e_install_begin.pkg;
-		pkg_fprintf(msg_buf->fp, "Installing %n-%v...\n", pkg,
+		pkg_fprintf(msg_buf->fp, BOLD "Installing %n-%v...\n" RESET, pkg,
 		    pkg);
 		fflush(msg_buf->fp);
 		printf("%s", msg_buf->buf);
@@ -627,7 +633,7 @@ event_callback(void *data, struct pkg_event *ev)
 		else {
 			job_status_begin(msg_buf);
 			pkg = ev->e_install_begin.pkg;
-			pkg_fprintf(msg_buf->fp, "Extracting %n-%v", pkg, pkg);
+			pkg_fprintf(msg_buf->fp, BOLD "Extracting %n-%v" RESET, pkg, pkg);
 			fflush(msg_buf->fp);
 		}
 		break;
@@ -642,12 +648,12 @@ event_callback(void *data, struct pkg_event *ev)
 	case PKG_EVENT_INTEGRITYCHECK_BEGIN:
 		if (quiet)
 			break;
-		printf("Checking integrity...");
+		printf(BOLD "Checking integrity..." RESET);
 		break;
 	case PKG_EVENT_INTEGRITYCHECK_FINISHED:
 		if (quiet)
 			break;
-		printf(" done (%d conflicting)\n", ev->e_integrity_finished.conflicting);
+		printf(BOLD " done (" COLOR_RED BOLD "%d " RESET BOLD "conflicting)\n" RESET, ev->e_integrity_finished.conflicting);
 		if (conflicts != NULL) {
 			fflush(conflicts->fp);
 			printf("%s", conflicts->buf);
@@ -658,7 +664,7 @@ event_callback(void *data, struct pkg_event *ev)
 	case PKG_EVENT_INTEGRITYCHECK_CONFLICT:
 		if (*debug == 0)
 			break;
-		printf("\nConflict found on path %s between %s and ",
+		printf(COLOR_RED BOLD "\nConflict found on path %s between %s and " RESET,
 		    ev->e_integrity_conflict.pkg_path,
 		    ev->e_integrity_conflict.pkg_uid);
 		cur_conflict = ev->e_integrity_conflict.conflicts;
@@ -679,7 +685,7 @@ event_callback(void *data, struct pkg_event *ev)
 		job_status_begin(msg_buf);
 
 		pkg = ev->e_install_begin.pkg;
-		pkg_fprintf(msg_buf->fp, "Deinstalling %n-%v...\n", pkg, pkg);
+		pkg_fprintf(msg_buf->fp, BOLD "Deinstalling %n-%v...\n" RESET, pkg, pkg);
 		fflush(msg_buf->fp);
 		printf("%s", msg_buf->buf);
 		break;
@@ -693,7 +699,7 @@ event_callback(void *data, struct pkg_event *ev)
 		else {
 			job_status_begin(msg_buf);
 			pkg = ev->e_install_begin.pkg;
-			pkg_fprintf(msg_buf->fp, "Deleting files for %n-%v",
+			pkg_fprintf(msg_buf->fp, BOLD "Deleting files for %n-%v" RESET,
 			    pkg, pkg);
 		}
 		break;
@@ -709,15 +715,15 @@ event_callback(void *data, struct pkg_event *ev)
 
 		switch (pkg_version_change_between(pkg_new, pkg_old)) {
 		case PKG_DOWNGRADE:
-			pkg_fprintf(msg_buf->fp, "Downgrading %n from %v to %v...\n",
+			pkg_fprintf(msg_buf->fp, BOLD "Downgrading %n from %v to %v...\n" RESET,
 			    pkg_new, pkg_old, pkg_new);
 			break;
 		case PKG_REINSTALL:
-			pkg_fprintf(msg_buf->fp, "Reinstalling %n-%v...\n",
+			pkg_fprintf(msg_buf->fp, BOLD "Reinstalling %n-%v...\n" RESET,
 		    pkg_old, pkg_old);
 			break;
 		case PKG_UPGRADE:
-			pkg_fprintf(msg_buf->fp, "Upgrading %n from %v to %v...\n",
+			pkg_fprintf(msg_buf->fp, BOLD "Upgrading %n from %v to %v...\n" RESET,
 			    pkg_new, pkg_old, pkg_new);
 			break;
 		}
@@ -730,75 +736,75 @@ event_callback(void *data, struct pkg_event *ev)
 		break;
 	case PKG_EVENT_LOCKED:
 		pkg = ev->e_locked.pkg;
-		pkg_printf("\n%n-%v is locked and may not be modified\n", pkg, pkg);
+		pkg_printf(COLOR_YELLOW BOLD "\n%n-%v is locked and may not be modified\n" RESET, pkg, pkg);
 		break;
 	case PKG_EVENT_REQUIRED:
 		pkg = ev->e_required.pkg;
-		pkg_printf("\n%n-%v is required by: %r%{%rn-%rv%| %}", pkg, pkg, pkg);
+		pkg_printf(COLOR_YELLOW BOLD "\n%n-%v is required by: %r%{%rn-%rv%| %}", pkg, pkg, pkg);
 		if (ev->e_required.force == 1)
-			fprintf(stderr, ", deleting anyway\n");
+			fprintf(stderr, ", deleting anyway\n" RESET);
 		else
-			fprintf(stderr, "\n");
+			fprintf(stderr, "\n" RESET);
 		break;
 	case PKG_EVENT_ALREADY_INSTALLED:
 		if (quiet)
 			break;
 		pkg = ev->e_already_installed.pkg;
-		pkg_printf("the most recent version of %n-%v is already installed\n",
+		pkg_printf(COLOR_YELLOW BOLD "the most recent version of " COLOR_GREEN BOLD "%n-%v " COLOR_YELLOW BOLD "is already installed\n" RESET,
 				pkg, pkg);
 		break;
 	case PKG_EVENT_NOT_FOUND:
-		printf("Package '%s' was not found in "
-		    "the repositories\n", ev->e_not_found.pkg_name);
+		printf(COLOR_RED BOLD "Package " COLOR_YELLOW BOLD "'%s' " COLOR_RED BOLD "was not found in "
+		    "the repositories\n" RESET, ev->e_not_found.pkg_name);
 		break;
 	case PKG_EVENT_MISSING_DEP:
-		warnx("Missing dependency '%s'",
+		warnx(COLOR_RED BOLD "Missing dependency " COLOR_YELLOW BOLD "'%s'" RESET,
 		    pkg_dep_name(ev->e_missing_dep.dep));
 		break;
 	case PKG_EVENT_NOREMOTEDB:
-		fprintf(stderr, "Unable to open remote database \"%s\". "
-		    "Try running '%s update' first.\n", ev->e_remotedb.repo,
+		fprintf(stderr, COLOR_RED BOLD "Unable to open remote database " COLOR_YELLOW BOLD "\"%s\". "
+		    COLOR_GREEN BOLD "Try running " COLOR_YELLOW BOLD "'%s update' " COLOR_GREEN BOLD "first.\n" RESET, ev->e_remotedb.repo,
 		    getprogname());
 		break;
 	case PKG_EVENT_NOLOCALDB:
-		fprintf(stderr, "Local package database nonexistent!\n");
+		fprintf(stderr, COLOR_RED BOLD "Local package database nonexistent!\n" RESET);
 		break;
 	case PKG_EVENT_NEWPKGVERSION:
 		newpkgversion = true;
-		printf("New version of pkg detected; it needs to be "
-		    "installed first.\n");
+		printf(COLOR_YELLOW BOLD "New version of pkg detected; it needs to be "
+		    "installed first.\n" RESET);
 		break;
 	case PKG_EVENT_FILE_MISMATCH:
 		pkg = ev->e_file_mismatch.pkg;
-		pkg_fprintf(stderr, "%n-%v: checksum mismatch for %Fn\n", pkg,
+		pkg_fprintf(stderr, COLOR_YELLOW BOLD "%n-%v: " COLOR_RED BOLD "checksum mismatch for " COLOR_YELLOW BOLD "%Fn\n" RESET, pkg,
 		    pkg, ev->e_file_mismatch.file);
 		break;
 	case PKG_EVENT_FILE_MISSING:
 		pkg = ev->e_file_missing.pkg;
-		pkg_fprintf(stderr, "%n-%v: missing file %Fn\n", pkg, pkg,
+		pkg_fprintf(stderr, COLOR_YELLOW BOLD "%n-%v: " COLOR_RED BOLD "missing file " COLOR_YELLOW BOLD "%Fn\n" RESET, pkg, pkg,
 		    ev->e_file_missing.file);
 		break;
 	case PKG_EVENT_PLUGIN_ERRNO:
-		warnx("%s: %s(%s): %s",
+		warnx(COLOR_RED BOLD "%s: %s(%s): %s" RESET,
 		    pkg_plugin_get(ev->e_plugin_errno.plugin, PKG_PLUGIN_NAME),
 		    ev->e_plugin_errno.func, ev->e_plugin_errno.arg,
 		    strerror(ev->e_plugin_errno.no));
 		break;
 	case PKG_EVENT_PLUGIN_ERROR:
-		warnx("%s: %s",
+		warnx(COLOR_RED BOLD "%s: %s" RESET,
 		    pkg_plugin_get(ev->e_plugin_error.plugin, PKG_PLUGIN_NAME),
 		    ev->e_plugin_error.msg);
 		break;
 	case PKG_EVENT_PLUGIN_INFO:
 		if (quiet)
 			break;
-		printf("%s: %s\n",
+		printf(COLOR_BLUE BOLD "%s: %s\n" RESET,
 		    pkg_plugin_get(ev->e_plugin_info.plugin, PKG_PLUGIN_NAME),
 		    ev->e_plugin_info.msg);
 		break;
 	case PKG_EVENT_INCREMENTAL_UPDATE:
 		if (!quiet)
-			printf("%s repository update completed. %d packages processed.\n",
+			printf(COLOR_BLUE BOLD "%s " COLOR_GREEN BOLD "repository update completed. " COLOR_BLUE BOLD "%d " COLOR_GREEN BOLD "packages processed.\n" RESET,
 			    ev->e_incremental_update.reponame,
 			    ev->e_incremental_update.processed);
 		break;
@@ -808,8 +814,8 @@ event_callback(void *data, struct pkg_event *ev)
 		break;
 	case PKG_EVENT_QUERY_YESNO:
 		return ( ev->e_query_yesno.deft ?
-			query_yesno(true, ev->e_query_yesno.msg, "[Y/n]") :
-			query_yesno(false, ev->e_query_yesno.msg, "[y/N]") );
+			query_yesno(true, ev->e_query_yesno.msg, COLOR_BLUE BOLD "[Y/n]" RESET) :
+			query_yesno(false, ev->e_query_yesno.msg, COLOR_BLUE BOLD "[y/N]" RESET) );
 		break;
 	case PKG_EVENT_QUERY_SELECT:
 		return query_select(ev->e_query_select.msg, ev->e_query_select.items,
@@ -834,10 +840,10 @@ event_callback(void *data, struct pkg_event *ev)
 		    ev->e_progress_tick.total);
 		break;
 	case PKG_EVENT_BACKUP:
-		fprintf(msg_buf->fp, "Backing up");
+		fprintf(msg_buf->fp, COLOR_GREEN BOLD "Backing up" RESET);
 		break;
 	case PKG_EVENT_RESTORE:
-		fprintf(msg_buf->fp, "Restoring");
+		fprintf(msg_buf->fp, COLOR_BLUE BOLD "Restoring" RESET);
 		break;
 	case PKG_EVENT_NEW_ACTION:
 		nbdone++;
@@ -845,7 +851,7 @@ event_callback(void *data, struct pkg_event *ev)
 	case PKG_EVENT_MESSAGE:
 		if (messages == NULL)
 			messages = xstring_new();
-		fprintf(messages->fp, "%s", ev->e_pkg_message.msg);
+		fprintf(messages->fp, BOLD "%s" RESET, ev->e_pkg_message.msg);
 		break;
 	case PKG_EVENT_CLEANUP_CALLBACK_REGISTER:
 		if (!signal_handler_installed) {
@@ -873,29 +879,29 @@ event_callback(void *data, struct pkg_event *ev)
 		if (conflicts == NULL) {
 			conflicts = xstring_new();
 		}
-		pkg_fprintf(conflicts->fp, "  - %n-%v",
+		pkg_fprintf(conflicts->fp, COLOR_YELLOW BOLD "  - %n-%v",
 		    ev->e_conflicts.p1, ev->e_conflicts.p1);
 		if (pkg_repos_total_count() > 1) {
 			pkg_get(ev->e_conflicts.p1, PKG_ATTR_REPONAME, &reponame);
-			fprintf(conflicts->fp, " [%s]",
+			fprintf(conflicts->fp, COLOR_YELLOW BOLD " [%s]" RESET,
 			    reponame == NULL ? "installed" : reponame);
 		}
-		pkg_fprintf(conflicts->fp, " conflicts with %n-%v",
+		pkg_fprintf(conflicts->fp, COLOR_RED BOLD " conflicts with " COLOR_YELLOW BOLD "%n-%v" RESET,
 		    ev->e_conflicts.p2, ev->e_conflicts.p2);
 		if (pkg_repos_total_count() > 1) {
 			pkg_get(ev->e_conflicts.p2, PKG_ATTR_REPONAME, &reponame);
-			fprintf(conflicts->fp, " [%s]",
+			fprintf(conflicts->fp, COLOR_YELLOW BOLD " [%s]" RESET,
 			    reponame == NULL ? "installed" : reponame);
 		}
-		fprintf(conflicts->fp, " on %s\n",
+		fprintf(conflicts->fp, COLOR_RED BOLD " on " COLOR_YELLOW BOLD "%s\n" RESET,
 		    ev->e_conflicts.path);
 		break;
 	case PKG_EVENT_TRIGGER:
 		if (!quiet) {
 			if (ev->e_trigger.cleanup)
-				printf("==> Cleaning up trigger: %s\n", ev->e_trigger.name);
+				printf(BOLD "==> " COLOR_BLUE BOLD "Cleaning up trigger: " COLOR_YELLOW BOLD "%s\n" RESET, ev->e_trigger.name);
 			else
-				printf("==> Running trigger: %s\n", ev->e_trigger.name);
+				printf(BOLD "==> " COLOR_GREEN BOLD "Running trigger: " COLOR_YELLOW BOLD "%s\n" RESET, ev->e_trigger.name);
 		}
 	default:
 		break;
