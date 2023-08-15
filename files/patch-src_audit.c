--- src/audit.c.orig	2023-08-01 08:25:14 UTC
+++ src/audit.c
@@ -58,6 +58,14 @@
 #include "xmalloc.h"
 #include "pkghash.h"
 
+#define RESET "\033[0m"
+#define BOLD "\033[1m"
+#define COLOR_GREEN "\033[32m"
+#define COLOR_RED "\033[31m"
+#define COLOR_YELLOW "\033[33m"
+#define COLOR_BLUE "\033[34m"
+#define COLOR_PURPLE "\033[35m"
+
 static const char* vop_names[] = {
 	[0] = "",
 	[EQ] = "=",
@@ -70,8 +78,8 @@ static const char* vop_names[] = {
 void
 usage_audit(void)
 {
-	fprintf(stderr, "Usage: pkg audit [-RFqr] [--raw[=format]|-R[format] [-f file] <pattern>\n\n");
-	fprintf(stderr, "For more information see 'pkg help audit'.\n");
+	fprintf(stderr, BOLD "Usage: pkg audit [-RFqr] [--raw[=format]|-R[format] [-f file] <pattern>\n\n");
+	fprintf(stderr, "For more information see 'pkg help audit'.\n" RESET);
 }
 
 static void
@@ -126,28 +134,28 @@ print_issue(struct pkg *p, struct pkg_audit_issue *iss
 
 	e = issue->audit;
 	if (version == NULL) {
-		printf("  Affected versions:\n");
+		printf(BOLD "  Affected versions:\n");
 		ll_foreach(e->versions, vers) {
 			if (vers->v1.type > 0 && vers->v2.type > 0)
-				printf("  %s %s : %s %s\n",
+				printf("  %s %s : %s %s\n" RESET,
 				    vop_names[vers->v1.type], vers->v1.version,
 				    vop_names[vers->v2.type], vers->v2.version);
 			else if (vers->v1.type > 0)
-				printf("  %s %s\n",
+				printf("  %s %s\n" RESET,
 				    vop_names[vers->v1.type], vers->v1.version);
 			else
-				printf("  %s %s\n",
+				printf("  %s %s\n" RESET,
 				    vop_names[vers->v2.type], vers->v2.version);
 		}
 	}
 	printf("  %s\n", e->desc);
 	ll_foreach(e->cve, cve) {
-		printf("  CVE: %s\n", cve->cvename);
+		printf(COLOR_YELLOW BOLD "  CVE: %s\n" RESET, cve->cvename);
 	}
 	if (e->url)
-		printf("  WWW: %s\n\n", e->url);
+		printf(COLOR_BLUE BOLD "  WWW: %s\n\n" RESET, e->url);
 	else if (e->id)
-		printf("  WWW: https://vuxml.FreeBSD.org/freebsd/%s.html\n\n", e->id);
+		printf(COLOR_BLUE BOLD "  WWW: https://vuxml.FreeBSD.org/freebsd/%s.html\n\n" RESET, e->id);
 }
 
 static void
@@ -276,11 +284,11 @@ exec_audit(int argc, char **argv)
 
 	if (pkg_audit_load(audit, audit_file) != EPKG_OK) {
 		if (errno == ENOENT)
-			warnx("vulnxml file %s does not exist. "
-					"Try running 'pkg audit -F' first",
+			warnx(COLOR_YELLOW BOLD "vulnxml file %s does not exist. "
+					"Try running 'pkg audit -F' first" RESET,
 					audit_file);
 		else
-			warn("unable to open vulnxml file %s",
+			warn(COLOR_RED BOLD "unable to open vulnxml file %s" RESET,
 					audit_file);
 
 		pkg_audit_free(audit);
@@ -318,12 +326,12 @@ exec_audit(int argc, char **argv)
 			pkghash_destroy(check);
 			return (EXIT_SUCCESS);
 		} else if (ret == EPKG_ENOACCESS) {
-			warnx("Insufficient privileges to read the package database");
+			warnx(COLOR_RED BOLD "Insufficient privileges to read the package database" RESET);
 			pkg_audit_free(audit);
 			pkghash_destroy(check);
 			return (EXIT_FAILURE);
 		} else if (ret != EPKG_OK) {
-			warnx("Error accessing the package database");
+			warnx(COLOR_RED BOLD "Error accessing the package database" RESET);
 			pkg_audit_free(audit);
 			pkghash_destroy(check);
 			return (EXIT_FAILURE);
@@ -339,12 +347,12 @@ exec_audit(int argc, char **argv)
 			pkgdb_close(db);
 			pkg_audit_free(audit);
 			pkghash_destroy(check);
-			warnx("Cannot get a read lock on a database, it is locked by another process");
+			warnx(COLOR_YELLOW BOLD "Cannot get a read lock on a database, it is locked by another process" RESET);
 			return (EXIT_FAILURE);
 		}
 
 		if ((it = pkgdb_query(db, NULL, MATCH_ALL)) == NULL) {
-			warnx("Error accessing the package database");
+			warnx(COLOR_RED BOLD "Error accessing the package database" RESET);
 			ret = EXIT_FAILURE;
 		}
 		else {
@@ -397,15 +405,15 @@ exec_audit(int argc, char **argv)
 					pkg_get(pkg, PKG_ATTR_VERSION, &version);
 					if (quiet) {
 						if (version != NULL)
-							pkg_printf("%n-%v\n", pkg, pkg);
+							pkg_printf(BOLD "%n-%v\n" RESET, pkg, pkg);
 							else
-						pkg_printf("%s\n", pkg);
+						pkg_printf(BOLD "%s\n" RESET, pkg);
 						continue;
 					}
 
 					pkg_printf("%n", pkg);
 					if (version != NULL)
-						pkg_printf("-%v", pkg);
+						pkg_printf(BOLD "-%v" RESET, pkg);
 					if (!quiet)
 						printf(" is vulnerable");
 					printf(":\n");
@@ -466,7 +474,7 @@ exec_audit(int argc, char **argv)
 
 		if (top == NULL) {
 			if (!quiet)
-				printf("%u problem(s) in %u installed package(s) found.\n",
+				printf(COLOR_YELLOW BOLD "%u " COLOR_BLUE "problem(s) in " COLOR_YELLOW "%u " COLOR_BLUE "installed package(s) found.\n" RESET,
 				   affected, vuln);
 	
 		} else {
@@ -476,7 +484,7 @@ exec_audit(int argc, char **argv)
 			ucl_object_unref(top);
 		}
 	} else {
-		warnx("cannot process vulnxml");
+		warnx(COLOR_RED BOLD "cannot process vulnxml" RESET);
 		ret = EXIT_FAILURE;
 		pkghash_destroy(check);
 	}
