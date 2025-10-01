#!/bin/bash

# ==========================================
# PHILOSOPHERS - MASSIVE FUNCTIONAL TESTER
# ==========================================

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

PASSED=0
FAILED=0
TOTAL=0

# Fonction pour tester
test_case() {
    local name=$1
    local philos=$2
    local die=$3
    local eat=$4
    local sleep=$5
    local meals=$6
    local should_die=$7
    local timeout_val=${8:-10}
    
    ((TOTAL++))
    
    if [ -z "$meals" ]; then
        output=$(timeout 5 ./philo $philos $die $eat $sleep 2>&1)
    else
        output=$(timeout 5 ./philo $philos $die $eat $sleep $meals 2>&1)
    fi
    
    if [ "$should_die" = "yes" ]; then
        if echo "$output" | grep -q "died"; then
            ((PASSED++))
            echo -e "${GREEN}✓${NC} $name"
        else
            ((FAILED++))
            echo -e "${RED}✗${NC} $name (devrait mourir)"
        fi
    else
        if echo "$output" | grep -q "died"; then
            ((FAILED++))
            echo -e "${RED}✗${NC} $name (mort inattendue)"
        else
            ((PASSED++))
            echo -e "${GREEN}✓${NC} $name"
        fi
    fi
}

echo -e "${CYAN}╔════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║        PHILOSOPHERS - MASSIVE FUNCTIONAL TESTER       ║${NC}"
echo -e "${CYAN}╚════════════════════════════════════════════════════════╝${NC}\n"

if [ ! -f "./philo" ]; then
    echo -e "${RED}Erreur: ./philo n'existe pas${NC}"
    exit 1
fi

# ==========================================
# PARTIE 1: CAS CLASSIQUES
# ==========================================
echo -e "${BLUE}═══ PARTIE 1: Cas classiques (15 tests) ═══${NC}"
test_case "1.01: 1 philo doit mourir" 1 800 200 200 "" "yes"
test_case "1.02: 2 philos survie" 2 800 200 200 "" "no"
test_case "1.03: 3 philos survie" 3 310 100 100 "" "no"
test_case "1.04: 4 philos survie" 4 410 200 200 "" "no"
test_case "1.05: 5 philos survie" 5 800 200 200 "" "no"
test_case "1.06: 6 philos survie" 6 800 200 100 "" "no"
test_case "1.07: 7 philos survie" 7 800 200 200 "" "no"
test_case "1.08: 10 philos survie" 10 800 200 200 "" "no"
test_case "1.09: 20 philos survie" 20 800 200 200 "" "no"
test_case "1.10: 50 philos survie" 50 800 200 200 "" "no"
test_case "1.11: 100 philos survie" 100 1000 200 200 "" "no"
test_case "1.12: 4 philos mort rapide" 4 310 200 100 "" "yes"
test_case "1.13: 5 philos avec repas" 5 800 200 200 7 "no"
test_case "1.14: 10 philos 1 repas" 10 800 200 200 1 "no"
test_case "1.15: 200 philos survie" 200 1500 300 200 "" "no"
echo ""

# ==========================================
# PARTIE 2: NOMBRES PAIRS
# ==========================================
echo -e "${BLUE}═══ PARTIE 2: Nombres pairs (10 tests) ═══${NC}"
test_case "2.01: 2 philos" 2 600 150 150 "" "no"
test_case "2.02: 4 philos" 4 500 150 150 "" "no"
test_case "2.03: 6 philos" 6 800 200 100 "" "no"
test_case "2.04: 8 philos" 8 800 200 200 "" "no"
test_case "2.05: 10 philos" 10 800 200 200 "" "no"
test_case "2.06: 20 philos" 20 800 200 200 "" "no"
test_case "2.07: 50 philos" 50 800 200 200 "" "no"
test_case "2.08: 100 philos" 100 1000 200 200 "" "no"
test_case "2.09: 150 philos" 150 1200 250 200 "" "no"
test_case "2.10: 200 philos max" 200 1500 300 200 "" "no"
echo ""

# ==========================================
# PARTIE 3: NOMBRES IMPAIRS
# ==========================================
echo -e "${BLUE}═══ PARTIE 3: Nombres impairs (10 tests) ═══${NC}"
test_case "3.01: 3 philos" 3 310 100 100 "" "no"
test_case "3.02: 5 philos" 5 800 200 200 "" "no"
test_case "3.03: 7 philos" 7 800 200 200 "" "no"
test_case "3.04: 9 philos" 9 800 200 200 "" "no"
test_case "3.05: 11 philos" 11 800 200 200 "" "no"
test_case "3.06: 21 philos" 21 800 200 200 "" "no"
test_case "3.07: 51 philos" 51 800 200 200 "" "no"
test_case "3.08: 101 philos" 101 1000 200 200 "" "no"
test_case "3.09: 151 philos" 151 1200 250 200 "" "no"
test_case "3.10: 199 philos" 199 1500 300 200 "" "no"
echo ""

# ==========================================
# PARTIE 4: TEMPS COURTS
# ==========================================
echo -e "${BLUE}═══ PARTIE 4: Temps courts (10 tests) ═══${NC}"
test_case "4.01: 5 philos 60/60" 5 300 60 60 "" "no"
test_case "4.02: 4 philos 80/80" 4 400 80 80 "" "no"
test_case "4.03: 6 philos 100/100" 6 500 100 100 "" "no"
test_case "4.04: 10 philos 100/100" 10 500 100 100 "" "no"
test_case "4.05: 5 philos 50/50" 5 300 50 50 "" "no"
test_case "4.06: 4 philos 70/70" 4 400 70 70 "" "no"
test_case "4.07: 3 philos 100/50" 3 400 100 50 "" "no"
test_case "4.08: 5 philos 90/60" 5 500 90 60 "" "no"
test_case "4.09: 7 philos 100/80" 7 500 100 80 "" "no"
test_case "4.10: 9 philos 120/80" 9 600 120 80 "" "no"
echo ""

# ==========================================
# PARTIE 5: TEMPS LONGS
# ==========================================
echo -e "${BLUE}═══ PARTIE 5: Temps longs (10 tests) ═══${NC}"
test_case "5.01: 5 philos 2000/500/500" 5 5000 2000 500 "" "no"
test_case "5.02: 4 philos 1500/400/400" 4 4000 1500 400 "" "no"
test_case "5.03: 6 philos 1000/300/300" 6 3000 1000 300 "" "no"
test_case "5.04: 3 philos 800/250/250" 3 2500 800 250 "" "no"
test_case "5.05: 10 philos 500/200/200" 10 2000 500 200 "" "no"
test_case "5.06: 5 philos 1200/400/300" 5 3500 1200 400 "" "no"
test_case "5.07: 7 philos 1000/350/250" 7 3000 1000 350 "" "no"
test_case "5.08: 4 philos 2500/600/400" 4 5000 2500 600 "" "no"
test_case "5.09: 8 philos 800/300/200" 8 2500 800 300 "" "no"
test_case "5.10: 20 philos 1000/250/250" 20 3000 1000 250 "" "no"
echo ""

# ==========================================
# PARTIE 6: ASYMÉTRIQUES
# ==========================================
echo -e "${BLUE}═══ PARTIE 6: Temps asymétriques (10 tests) ═══${NC}"
test_case "6.01: Eat > Sleep (5 800 300 100)" 5 1200 800 300 "" "no"
test_case "6.02: Eat > Sleep (4 800 400 100)" 4 1200 800 400 "" "no"
test_case "6.03: Sleep > Eat (5 800 100 300)" 5 1200 100 300 "" "no"
test_case "6.04: Sleep > Eat (6 800 150 350)" 6 1200 150 350 "" "no"
test_case "6.05: Eat >> Sleep (5 800 500 50)" 5 1500 800 500 "" "no"
test_case "6.06: Sleep >> Eat (5 800 50 500)" 5 1500 50 500 "" "no"
test_case "6.07: Eat 2x Sleep (4 800 200 400)" 4 1200 200 400 "" "no"
test_case "6.08: Eat 3x Sleep (5 900 300 900)" 5 2500 300 900 "" "no"
test_case "6.09: Eat 4x Sleep (3 800 100 400)" 3 1500 100 400 "" "no"
test_case "6.10: Très déséquilibré (6 1000 50 800)" 6 2500 50 800 "" "no"
echo ""

# ==========================================
# PARTIE 7: AVEC NOMBRE DE REPAS
# ==========================================
echo -e "${BLUE}═══ PARTIE 7: Avec nombre de repas (15 tests) ═══${NC}"
test_case "7.01: 5 philos 1 repas" 5 800 200 200 1 "no"
test_case "7.02: 5 philos 2 repas" 5 800 200 200 2 "no"
test_case "7.03: 5 philos 5 repas" 5 800 200 200 5 "no"
test_case "7.04: 5 philos 7 repas" 5 800 200 200 7 "no"
test_case "7.05: 5 philos 10 repas" 5 800 200 200 10 "no"
test_case "7.06: 4 philos 3 repas" 4 800 200 200 3 "no"
test_case "7.07: 6 philos 4 repas" 6 800 200 200 4 "no"
test_case "7.08: 10 philos 2 repas" 10 800 200 200 2 "no"
test_case "7.09: 3 philos 5 repas" 3 500 100 100 5 "no"
test_case "7.10: 2 philos 10 repas" 2 800 200 200 10 "no"
test_case "7.11: 20 philos 1 repas" 20 800 200 200 1 "no"
test_case "7.12: 50 philos 2 repas" 50 800 200 200 2 "no"
test_case "7.13: 7 philos 15 repas" 7 800 200 200 15 "no"
test_case "7.14: 4 philos 20 repas" 4 800 200 200 20 "no"
test_case "7.15: 100 philos 1 repas" 100 1000 200 200 1 "no"
echo ""

# ==========================================
# PARTIE 8: MORTS ATTENDUES
# ==========================================
echo -e "${BLUE}═══ PARTIE 8: Cas de mort attendue (10 tests) ═══${NC}"
test_case "8.01: 1 philo 800ms" 1 800 200 200 "" "yes"
test_case "8.02: 1 philo 1000ms" 1 1000 300 300 "" "yes"
test_case "8.03: 4 philos die=310" 4 310 200 100 "" "yes"
test_case "8.04: 5 philos die=300" 5 300 200 100 "" "yes"
test_case "8.05: 3 philos die=250" 3 250 150 100 "" "yes"
test_case "8.06: 2 philos die=400" 2 400 300 200 "" "yes"
test_case "8.07: 6 philos die=500" 6 500 300 250 "" "yes"
test_case "8.08: 4 philos die=350" 4 350 200 150 "" "yes"
test_case "8.09: 10 philos die=600" 10 600 400 300 "" "yes"
test_case "8.10: 1 philo 500ms" 1 500 150 150 "" "yes"
echo ""

# ==========================================
# PARTIE 9: CAS LIMITES
# ==========================================
echo -e "${BLUE}═══ PARTIE 9: Cas limites/edge cases (15 tests) ═══${NC}"
test_case "9.01: 2 philos die=eat+sleep" 2 400 200 200 "" "no"
test_case "9.02: 3 philos die=eat+sleep" 3 300 150 150 "" "no"
test_case "9.03: 4 philos die=eat+sleep+10" 4 410 200 200 "" "no"
test_case "9.04: 5 philos die=eat+sleep+50" 5 450 200 200 "" "no"
test_case "9.05: 4 philos eat=sleep" 4 600 200 200 "" "no"
test_case "9.06: 5 philos eat=die/2" 5 800 400 200 "" "no"
test_case "9.07: 6 philos sleep=die/2" 6 800 200 400 "" "no"
test_case "9.08: 199 philos (max-1)" 199 1500 300 200 "" "no"
test_case "9.09: 200 philos (max)" 200 1500 300 200 "" "no"
test_case "9.10: 10 philos eat=1ms" 10 1000 1 1 "" "no"
test_case "9.11: 5 philos sleep=1ms" 5 1000 200 1 "" "no"
test_case "9.12: 4 philos die=3000" 4 3000 500 500 "" "no"
test_case "9.13: 3 philos 100 repas" 3 800 200 200 100 "no"
test_case "9.14: 2 philos 50 repas" 2 800 200 200 50 "no"
test_case "9.15: 10 philos temps=1" 10 500 1 1 "" "no"
echo ""

# ==========================================
# PARTIE 10: STRESS ALÉATOIRE
# ==========================================
echo -e "${BLUE}═══ PARTIE 10: Combinaisons variées (20 tests) ═══${NC}"
test_case "10.01: Config 1" 7 900 220 180 "" "no"
test_case "10.02: Config 2" 13 1100 250 200 "" "no"
test_case "10.03: Config 3" 17 950 210 190 "" "no"
test_case "10.04: Config 4" 23 1050 230 180 "" "no"
test_case "10.05: Config 5" 11 850 205 195 "" "no"
test_case "10.06: Config 6" 15 920 215 185 "" "no"
test_case "10.07: Config 7" 19 980 225 175 "" "no"
test_case "10.08: Config 8" 12 870 208 192 "" "no"
test_case "10.09: Config 9" 14 940 218 182 "" "no"
test_case "10.10: Config 10" 16 1020 228 172 "" "no"
test_case "10.11: Config 11" 25 1150 260 190 "" "no"
test_case "10.12: Config 12" 31 1250 280 170 "" "no"
test_case "10.13: Config 13" 41 1350 300 200 "" "no"
test_case "10.14: Config 14" 37 1200 270 180 "" "no"
test_case "10.15: Config 15" 43 1400 310 190 "" "no"
test_case "10.16: Config 16" 27 1180 265 185 "" "no"
test_case "10.17: Config 17" 33 1280 285 175 "" "no"
test_case "10.18: Config 18" 39 1320 295 195 "" "no"
test_case "10.19: Config 19" 29 1220 275 165 "" "no"
test_case "10.20: Config 20" 35 1300 290 210 "" "no"
echo ""

# ==========================================
# RÉSUMÉ
# ==========================================
echo -e "${CYAN}╔════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                   RÉSUMÉ FINAL                         ║${NC}"
echo -e "${CYAN}╚════════════════════════════════════════════════════════╝${NC}\n"

PERCENTAGE=$((PASSED * 100 / TOTAL))

echo -e "Total de tests : ${CYAN}$TOTAL${NC}"
echo -e "Tests réussis  : ${GREEN}$PASSED${NC}"
echo -e "Tests échoués  : ${RED}$FAILED${NC}"
echo -e "Taux de réussite : ${CYAN}$PERCENTAGE%${NC}\n"

if [ "$FAILED" -eq 0 ]; then
    echo -e "${GREEN}╔════════════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║         TOUS LES 125 TESTS SONT PASSÉS !              ║${NC}"
    echo -e "${GREEN}║              TON CODE EST PARFAIT !                    ║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════════════════════╝${NC}\n"
elif [ "$PERCENTAGE" -ge 95 ]; then
    echo -e "${GREEN}Excellent ! Presque tous les tests passent.${NC}\n"
elif [ "$PERCENTAGE" -ge 85 ]; then
    echo -e "${YELLOW}Très bien ! Quelques cas à ajuster.${NC}\n"
elif [ "$PERCENTAGE" -ge 70 ]; then
    echo -e "${YELLOW}Pas mal, mais il y a des problèmes à corriger.${NC}\n"
else
    echo -e "${RED}Plusieurs problèmes détectés.${NC}\n"
fi

exit $FAILED