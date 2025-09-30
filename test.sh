#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${BLUE}╔════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║   PHILOSOPHERS - TESTS AVANCÉS        ║${NC}"
echo -e "${BLUE}╔════════════════════════════════════════╗${NC}\n"

# Vérifier que le programme existe
if [ ! -f "./philo" ]; then
    echo -e "${RED}Erreur: ./philo n'existe pas. Compile d'abord avec 'make'${NC}"
    exit 1
fi

# Vérifier que valgrind est installé
if ! command -v valgrind &> /dev/null; then
    echo -e "${YELLOW}⚠ Valgrind n'est pas installé. Installation...${NC}"
    echo -e "${YELLOW}Sur Ubuntu/Debian: sudo apt-get install valgrind${NC}"
    echo -e "${YELLOW}Sur MacOS: brew install valgrind${NC}"
fi

# ============================================
# TESTS BASIQUES DE FONCTIONNEMENT
# ============================================
echo -e "${BLUE}═══ PARTIE 1: Tests de fonctionnement ═══${NC}\n"

# Test 1
echo -e "${BLUE}Test 1: 4 310 200 100 (doit mourir)${NC}"
output=$(timeout 2 ./philo 4 310 200 100 2>&1)
if echo "$output" | grep -q "died"; then
    echo -e "${GREEN}✓ PASS - Mort détectée${NC}"
else
    echo -e "${RED}✗ FAIL - Aucune mort${NC}"
fi
echo ""

# Test 2
echo -e "${BLUE}Test 2: 4 410 200 200 (ne doit pas mourir)${NC}"
output=$(timeout 3 ./philo 4 410 200 200 2>&1)
if echo "$output" | grep -q "died"; then
    echo -e "${RED}✗ FAIL - Mort inattendue${NC}"
else
    echo -e "${GREEN}✓ PASS - Aucune mort${NC}"
fi
echo ""

# Test 3
echo -e "${BLUE}Test 3: 5 800 200 200 (ne doit pas mourir)${NC}"
output=$(timeout 3 ./philo 5 800 200 200 2>&1)
if echo "$output" | grep -q "died"; then
    echo -e "${RED}✗ FAIL - Mort inattendue${NC}"
else
    echo -e "${GREEN}✓ PASS - Aucune mort${NC}"
fi
echo ""

# Test 4 - TON CAS
echo -e "${BLUE}Test 4: 6 800 200 100 (ne doit pas mourir - TON CAS)${NC}"
output=$(timeout 5 ./philo 6 800 200 100 2>&1)
if echo "$output" | grep -q "died"; then
    echo -e "${RED}✗ FAIL - Mort inattendue${NC}"
else
    echo -e "${GREEN}✓ PASS - Aucune mort${NC}"
fi
echo ""

# Test 5
echo -e "${BLUE}Test 5: 1 800 200 200 (doit mourir)${NC}"
output=$(timeout 2 ./philo 1 800 200 200 2>&1)
if echo "$output" | grep -q "died"; then
    echo -e "${GREEN}✓ PASS - Mort détectée${NC}"
else
    echo -e "${RED}✗ FAIL - Pas de mort${NC}"
fi
echo ""

# Test 6
echo -e "${BLUE}Test 6: 2 800 200 200 (ne doit pas mourir)${NC}"
output=$(timeout 3 ./philo 2 800 200 200 2>&1)
if echo "$output" | grep -q "died"; then
    echo -e "${RED}✗ FAIL - Mort inattendue${NC}"
else
    echo -e "${GREEN}✓ PASS - Aucune mort${NC}"
fi
echo ""

# Test 7 avec repas
echo -e "${BLUE}Test 7: 5 800 200 200 7 (arrêt propre après 7 repas)${NC}"
output=$(timeout 10 ./philo 5 800 200 200 7 2>&1)
if echo "$output" | grep -q "died"; then
    echo -e "${RED}✗ FAIL - Mort inattendue${NC}"
else
    echo -e "${GREEN}✓ PASS - Arrêt propre${NC}"
fi
echo ""

# Test 8
echo -e "${BLUE}Test 8: 200 800 200 200 (stress test)${NC}"
output=$(timeout 3 ./philo 200 800 200 200 2>&1)
if echo "$output" | grep -q "died"; then
    echo -e "${RED}✗ FAIL - Mort inattendue${NC}"
else
    echo -e "${GREEN}✓ PASS - Aucune mort${NC}"
fi
echo ""

# ============================================
# TESTS DE MESSAGES APRÈS LA MORT
# ============================================
echo -e "\n${BLUE}═══ PARTIE 2: Pas de message après la mort ═══${NC}\n"

echo -e "${BLUE}Test 9: Vérifier qu'aucun message n'apparaît après 'died'${NC}"
output=$(timeout 2 ./philo 4 310 200 100 2>&1)
died_line=$(echo "$output" | grep -n "died" | head -1 | cut -d: -f1)
if [ ! -z "$died_line" ]; then
    total_lines=$(echo "$output" | wc -l)
    if [ "$died_line" -eq "$total_lines" ]; then
        echo -e "${GREEN}✓ PASS - Aucun message après la mort${NC}"
    else
        after_death=$(echo "$output" | tail -n +$((died_line + 1)))
        echo -e "${RED}✗ FAIL - Messages après la mort:${NC}"
        echo "$after_death"
    fi
else
    echo -e "${YELLOW}⚠ SKIP - Pas de mort détectée${NC}"
fi
echo ""

# ============================================
# TESTS DES TIMESTAMPS
# ============================================
echo -e "\n${BLUE}═══ PARTIE 3: Vérification des timestamps ═══${NC}\n"

echo -e "${BLUE}Test 10: Les timestamps doivent être croissants${NC}"
output=$(timeout 2 ./philo 5 800 200 200 2>&1 | head -100)
timestamps=$(echo "$output" | awk '{print $1}')
is_sorted=$(echo "$timestamps" | awk 'NR>1 && $1<prev {print "NO"; exit} {prev=$1} END {print "YES"}')
if [ "$is_sorted" = "YES" ]; then
    echo -e "${GREEN}✓ PASS - Timestamps en ordre croissant${NC}"
else
    echo -e "${RED}✗ FAIL - Timestamps désordonnés${NC}"
    echo "$output" | head -20
fi
echo ""

echo -e "${BLUE}Test 11: Pas de timestamp > 10ms après time_to_die pour le cas 1 philo${NC}"
output=$(timeout 2 ./philo 1 800 200 200 2>&1)
death_time=$(echo "$output" | grep "died" | awk '{print $1}')
if [ ! -z "$death_time" ]; then
    if [ "$death_time" -le 810 ]; then
        echo -e "${GREEN}✓ PASS - Mort à ${death_time}ms (attendu ~800ms)${NC}"
    else
        echo -e "${RED}✗ FAIL - Mort trop tardive: ${death_time}ms${NC}"
    fi
else
    echo -e "${RED}✗ FAIL - Pas de mort détectée${NC}"
fi
echo ""

# ============================================
# TESTS VALGRIND - MEMORY LEAKS
# ============================================
echo -e "\n${BLUE}═══ PARTIE 4: Valgrind - Memory Leaks ═══${NC}\n"

if command -v valgrind &> /dev/null; then
    echo -e "${BLUE}Test 12: Détection de memory leaks avec valgrind${NC}"
    echo -e "${YELLOW}(Ceci peut prendre 30 secondes...)${NC}"
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
             --log-file=valgrind_leaks.log \
             ./philo 5 800 200 200 7 > /dev/null 2>&1
    
    leaks=$(grep "definitely lost" valgrind_leaks.log | awk '{print $4}')
    if [ "$leaks" = "0" ]; then
        echo -e "${GREEN}✓ PASS - Aucun memory leak${NC}"
    else
        echo -e "${RED}✗ FAIL - Memory leaks détectés: ${leaks} bytes${NC}"
        echo -e "${YELLOW}Voir valgrind_leaks.log pour plus de détails${NC}"
    fi
    echo ""
else
    echo -e "${YELLOW}⚠ SKIP - Valgrind non installé${NC}\n"
fi

# ============================================
# TESTS HELGRIND - DATA RACES
# ============================================
echo -e "\n${BLUE}═══ PARTIE 5: Helgrind - Data Races ═══${NC}\n"

if command -v valgrind &> /dev/null; then
    echo -e "${BLUE}Test 13: Détection de data races avec helgrind${NC}"
    echo -e "${YELLOW}(Ceci peut prendre 1-2 minutes...)${NC}"
    valgrind --tool=helgrind --log-file=helgrind.log \
             ./philo 5 800 200 200 7 > /dev/null 2>&1
    
    races=$(grep -c "Possible data race" helgrind.log)
    if [ "$races" -eq 0 ]; then
        echo -e "${GREEN}✓ PASS - Aucune data race détectée${NC}"
    else
        echo -e "${RED}✗ FAIL - ${races} data races détectées${NC}"
        echo -e "${YELLOW}Voir helgrind.log pour plus de détails${NC}"
        echo -e "\n${YELLOW}Premières data races trouvées:${NC}"
        grep -A 5 "Possible data race" helgrind.log | head -20
    fi
    echo ""
else
    echo -e "${YELLOW}⚠ SKIP - Valgrind non installé${NC}\n"
fi

# ============================================
# TESTS DRD - DATA RACES (alternative)
# ============================================
echo -e "\n${BLUE}═══ PARTIE 6: DRD - Data Races (alternative) ═══${NC}\n"

if command -v valgrind &> /dev/null; then
    echo -e "${BLUE}Test 14: Détection de data races avec DRD${NC}"
    echo -e "${YELLOW}(Ceci peut prendre 1-2 minutes...)${NC}"
    valgrind --tool=drd --log-file=drd.log \
             ./philo 5 800 200 200 7 > /dev/null 2>&1
    
    races=$(grep -c "Conflicting" drd.log)
    if [ "$races" -eq 0 ]; then
        echo -e "${GREEN}✓ PASS - Aucune data race détectée${NC}"
    else
        echo -e "${RED}✗ FAIL - ${races} conflits détectés${NC}"
        echo -e "${YELLOW}Voir drd.log pour plus de détails${NC}"
    fi
    echo ""
else
    echo -e "${YELLOW}⚠ SKIP - Valgrind non installé${NC}\n"
fi

# ============================================
# TESTS DE STRESS
# ============================================
echo -e "\n${BLUE}═══ PARTIE 7: Tests de stress ═══${NC}\n"

echo -e "${BLUE}Test 15: Stress test - 100 itérations rapides${NC}"
echo -e "${YELLOW}(Peut prendre 1 minute...)${NC}"
failed=0
for i in {1..100}; do
    output=$(timeout 1 ./philo 5 800 200 200 2>&1)
    if echo "$output" | grep -q "died"; then
        ((failed++))
    fi
done
if [ "$failed" -eq 0 ]; then
    echo -e "${GREEN}✓ PASS - 100/100 tests réussis${NC}"
else
    echo -e "${RED}✗ FAIL - ${failed}/100 tests ont échoué (mort inattendue)${NC}"
fi
echo ""

echo -e "${BLUE}Test 16: Temps d'exécution avec beaucoup de philos${NC}"
start=$(date +%s%N)
timeout 5 ./philo 200 800 200 200 > /dev/null 2>&1
end=$(date +%s%N)
elapsed=$(( (end - start) / 1000000 ))
echo -e "${GREEN}Temps écoulé: ${elapsed}ms${NC}"
if [ "$elapsed" -lt 5000 ]; then
    echo -e "${GREEN}✓ PASS - Pas de deadlock${NC}"
else
    echo -e "${RED}✗ FAIL - Possible deadlock ou trop lent${NC}"
fi
echo ""

# ============================================
# TESTS D'ARGUMENTS INVALIDES
# ============================================
echo -e "\n${BLUE}═══ PARTIE 8: Gestion des erreurs ═══${NC}\n"

echo -e "${BLUE}Test 17: Arguments invalides${NC}"

# Trop peu d'arguments
./philo 5 800 200 > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo -e "${GREEN}✓ PASS - Rejet des arguments incomplets${NC}"
else
    echo -e "${RED}✗ FAIL - Devrait rejeter les arguments incomplets${NC}"
fi

# Nombre négatif
./philo -5 800 200 200 > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo -e "${GREEN}✓ PASS - Rejet des nombres négatifs${NC}"
else
    echo -e "${RED}✗ FAIL - Devrait rejeter les nombres négatifs${NC}"
fi

# Zéro philosophe
./philo 0 800 200 200 > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ PASS - Gestion du cas 0 philo${NC}"
else
    echo -e "${YELLOW}⚠ INFO - Le programme a retourné une erreur pour 0 philo${NC}"
fi
echo ""

# ============================================
# RÉSUMÉ FINAL
# ============================================
echo -e "\n${BLUE}╔════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║         TESTS TERMINÉS                 ║${NC}"
echo -e "${BLUE}╔════════════════════════════════════════╗${NC}\n"

echo -e "${YELLOW}Fichiers de logs générés:${NC}"
echo -e "  - valgrind_leaks.log (memory leaks)"
echo -e "  - helgrind.log (data races - helgrind)"
echo -e "  - drd.log (data races - drd)"
echo -e ""
echo -e "${BLUE}Pour voir les détails:${NC}"
echo -e "  cat valgrind_leaks.log"
echo -e "  cat helgrind.log"
echo -e "  cat drd.log"
echo -e ""